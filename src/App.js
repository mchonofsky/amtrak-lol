import React, {useState} from 'react';
import Mainbox from './components/Mainbox.js'
import './App.css';
import AppStateClass, {GlobalContext } from './context/GlobalContext.js'

function App() {
  const [appState, setAppState ] = useState( new AppStateClass() );
  
  return (
    <div class='app'>
      <div class="splashbox">
        <div id="wherethe">where the</div>
        <div class="fuck">FUCK</div>
        <div>is my train</div>
      </div>
      <GlobalContext.Provider value={[ appState, setAppState ]}>
        <Mainbox />
      </GlobalContext.Provider>
    </div>
  );
}

export default App;
