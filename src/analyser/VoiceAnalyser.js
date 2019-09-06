window.AudioContext = window.AudioContext || window.webkitAudioContext;

class VoiceAnalyser {

  constructor() {
    this.context = new AudioContext();

    this.gain = this.context.createGain();

    navigator.mediaDevices.getUserMedia({audio: true})
      .then(stream => {
        if (this.stream && this.source) {
          this.source.disconnect();
        }
        this.stream = stream;
        this.source = this.context.createMediaStreamSource(stream);
        this.source.connect(this.gain);
      })
      .catch(ex => {
        console.error("Error capturing audio.", ex);
      });
  }

  loadModules() {
    return Promise.all([
      this.context.audioWorklet.addModule('static/wasm/lpcAnalyser.js'),
    ]).then(this.createWorkletNodes.bind(this));
  }

  createWorkletNodes() {
    this.lpcAnalyser = new AudioWorkletNode(this.context, 'LPCAnalyser', {
      numberOfInputs: 1,
      numberOfOutputs: 0,
      channelCount: 1
    });

    this.gain.connect(this.lpcAnalyser);
  }

  requestData(frequencies) {
    this.lpcAnalyser.port.postMessage({type: 'getData', frequencies});
  }

  setDataCallback(callback) {
    this.lpcAnalyser.port.onmessage = ({data}) => callback(data);
  }

}

export default VoiceAnalyser;
