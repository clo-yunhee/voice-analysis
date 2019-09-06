class LPCAnalyser extends AudioWorkletProcessor {

  constructor() {
    super();

    this.heapInputBuffer = new HeapAudioBuffer(
        AnalyserModule, RENDER_QUANTUM_FRAMES,
        2, MAX_CHANNEL_COUNT
    );

    this.kernel = new AnalyserModule.LPCAnalyser();

    // Listen for a plot data request
    this.port.onmessage = this.onMessage.bind(this);
  }

  onMessage({data}) {
    if (data.type === 'getData') {
      this.sendData(data.frequencies);
    }
    else {
      console.warn(`MessagePort event type ${data.type} does not exist.`, data);
    }
  }

  sendData(frequencies) {
    const data = this.kernel.getData(frequencies);
    this.port.postMessage(data);
  }

  process(inputs, outputs) {
    const input = inputs[0];

    const channelCount = input.length;

    this.heapInputBuffer.adaptChannel(channelCount);

    for (let ch = 0; ch < channelCount; ++ch) {
      this.heapInputBuffer.getChannelData(ch).set(input[ch]);
    }

    this.kernel.process(this.heapInputBuffer.getHeapAddress(), channelCount);

    return true;
  }

}

registerProcessor("LPCAnalyser", LPCAnalyser);
