import React from 'react';
import Grid from '@material-ui/core/Grid';
import './App.css';
import VoiceAnalyser from './analyser/VoiceAnalyser'
import Footer from "./components/Footer";

class App extends React.PureComponent {

  constructor(props) {
    super(props);
  }

  componentDidMount() {
    this.analyser = new VoiceAnalyser();

    const callback = () => {
      window.requestAnimationFrame(callback);
      this.analyser.drawSpectrogram(this.canvas, 'wideband');
    };
    callback();
  }

  render() {
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
                <canvas id="canvas" ref={r => this.canvas = r} width={854} height={480}/>
              </Grid>
            </Grid>
          </div>
          <Footer/>
        </div>
    );
  }
}

export default App;
