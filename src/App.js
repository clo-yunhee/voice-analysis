import React from 'react';
import Grid from '@material-ui/core/Grid';
import Typography from '@material-ui/core/Typography';
import {HorizontalGridLines, VerticalGridLines, LineSeries, XAxis, YAxis, XYPlot} from "react-vis";
import './App.css';
import VoiceAnalyser from './analyser/VoiceAnalyser'
import Footer from "./components/Footer";

const minFreq = 150;
const maxFreq = 8000;
const plotWidth = 576;
const plotHeight = 360;

export const plotNbPoints = plotWidth;
export const frequencies = new Float32Array(plotNbPoints);

// Init with log10 scale.
const logMinFreq = Math.log10(minFreq);
const logMaxFreq = Math.log10(maxFreq);

for (let k = 0; k < plotNbPoints; ++k) {
  frequencies[k] = 10 ** (logMinFreq + (k * (logMaxFreq - logMinFreq)) / plotWidth);
}

function formatTickHz(f) {
  const log2 = Math.log10(2);
  const log4 = Math.log10(0.5);
  const logf = Math.log10(f);

  if (logf % 1 === 0
      || (log2 + logf) % 1 === 0
      || (log4 + logf) % 1 === 0) {

    if (f < 1000) {
      return f + " Hz";
    } else {
      return (f / 1000) + " kHz";
    }
  }
}

function formatTickDecibel(amp) {
  const gain = 20 * Math.log10(amp);

  if (gain % 20 === 0) {
    return gain + " dB";
  }
}


class App extends React.PureComponent {

  constructor(props) {
    super(props);
    this.state = {
      lpcResponse: []
    }
  }

  componentDidMount() {
    this.analyser = new VoiceAnalyser();
    this.analyser.loadModules().then(() => {
      this.analyser.setDataCallback(this.updateState.bind(this));

      const callback = () => {
        this.analyser.requestData(frequencies);

        window.setTimeout(callback, 200);
      };

      callback();
    });
  }

  updateState(r) {
    const { response, pitch } = r;

    this.setState({lpcResponse: response, pitch});
  }

  render() {
    const {
        lpcResponse,
        pitch
    } = this.state;

    return (
        <div className="App">
          <div className="App-wrapper">
            <Grid
                container
                spacing={4}
                direction="column"
                alignItems="flex-start"
                alignContent="flex-start"
                className="App-container"
            >
              <Grid item>
                <Typography variant="body1">
                  Pitch: <b>{pitch}</b>
                </Typography>
              </Grid>
              <Grid item>
                <XYPlot
                    width={plotWidth}
                    height={plotHeight}
                    xType="log"
                    xDomain={[minFreq, maxFreq]}
                    yType="log"
                    yDomain={[1e-6, 1]}
                    margin={{top: 30, left: 60, right: 20, bottom: 20}}
                >
                  <HorizontalGridLines/>
                  <VerticalGridLines/>
                  <XAxis title="Frequency (Hz)" top={0} orientation="top" tickFormat={formatTickHz}/>
                  <YAxis title="Gain (dB)" position="start" tickFormat={formatTickDecibel}/>
                  <LineSeries
                      color="transparent"
                      stroke="orange"
                      strokeWidth={1}
                      data={lpcResponse}
                  />
                </XYPlot>

              </Grid>
            </Grid>
          </div>
          <Footer/>
        </div>
    );
  }
}

export default App;
