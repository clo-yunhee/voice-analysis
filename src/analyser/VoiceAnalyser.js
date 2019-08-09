import {db2gain} from '../gainConversion'

window.AudioContext = window.AudioContext || window.webkitAudioContext;

class VoiceAnalyser {

  constructor() {
    this.context = new AudioContext();

    this.gain = this.context.createGain();

    this.compressor = this.context.createDynamicsCompressor();
    this.compressor.threshold.setValueAtTime(-30, this.context.currentTime);
    this.compressor.knee.setValueAtTime(35, this.context.currentTime);
    this.compressor.ratio.setValueAtTime(3.7, this.context.currentTime);
    this.compressor.attack.setValueAtTime(0.10, this.context.currentTime);
    this.compressor.release.setValueAtTime(1.0, this.context.currentTime);

    this.analyser = this.context.createAnalyser();
    this.analyser.fftSize = 4096;
    this.analyser.smoothingTimeConstant = 0.3;

    this.gain.connect(this.compressor);
    this.compressor.connect(this.analyser);

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

  async drawSpectrogram(canvas, specType) {
    if (specType !== 'wideband' && specType !== 'narrowband') {
      console.error('Unknown spectrogram type');
      return;
    }

    const ctx = canvas.getContext("2d");

    const binCount = this.analyser.frequencyBinCount;
    const spectrum = new Float32Array(binCount);

    this.analyser.getFloatFrequencyData(spectrum);

    const minFreq = 0;
    const maxFreq = 6000; // Only get freqs below 6 kHz

    const freqStep = 22050 / binCount;

    ctx.clearRect(0, 0, canvas.width, canvas.height);

    ctx.fillStyle = 'black';

    const maxN = maxFreq / 22050 * binCount;

    for (let n = 0; n < maxN; ++n) {
      const curFreq = minFreq + n * 22050 / binCount;

      const height = 720 * db2gain(spectrum[n]);

      ctx.fillRect(curFreq / maxFreq * canvas.width, canvas.height - height, freqStep / maxFreq * canvas.width, height);
    }

    ctx.lineWidth = 4;
    ctx.strokeStyle = 'blue';
    ctx.strokeRect(0, 0, canvas.width, canvas.height);
  }

}

export default VoiceAnalyser;
