import React, { useState, useContext } from 'react';                                                             
import {GlobalContext } from '../context/GlobalContext.js'
import axios from 'axios';
import {makeSwitchTo, switchTo} from '../context/setup.js';

function LocationPrompt(props) {
  const [appState, setAppState ] = useContext(GlobalContext );
  
  // TODO merge into appState
  const [errorPromptVisible, setErrorPromptVisible ] = useState(false);

  const errorPromptInvisible = () => {
    setErrorPromptVisible(false);
  }

  const getLocation = () => {
    const options = { enableHighAccuracy: true, timeout: 15000};
    const doError = (err) => 
      {
        console.log(err);
        setErrorPromptVisible(true);
        setTimeout(errorPromptInvisible, 3000);
      };
    const doSuccess = async (result) => {
      console.log('in doSuccess');
      // gets geolocation
      console.log('location is', result);
      appState.geolocation = result;
      setAppState({...appState, 'geolocation': appState.geolocation})
      try {
        let response = await axios.post('/api/get_train', {
          coords: appState.geolocation.coords,
          timestamp: appState.geolocation.timestamp
        })
        // then retrieves train
        appState.trains = response.data;
        if ( appState.trains.length > 1 ) {
          switchTo('multiple-trains', appState, setAppState);
        } else if ( appState.trains.length === 1 ) {
          switchTo('time-display', appState, setAppState);
        }
      }
      catch (e) {
        doError(e);
      }
    }


    if ( 'geolocation' in navigator ) {
      navigator.geolocation.getCurrentPosition(doSuccess, doError, options );
    } 
    else
    {
      // todo: custom error if geoloc is not supported
      doError('no geolocation available');
    }
  }
   
  return ( <>
    <div class="mainbox-box first">
      <div class="top-header">Where are you?</div>
      <div class="morebutton stops" onClick={getLocation}>use my location</div>
      {errorPromptVisible && 
        (
          <div class="error-prompt">We couldn't get your location. You can keep trying.</div>
        )
      }
    </div>
    <div class="mainbox-box">
      <div class="morebutton trains" onClick={makeSwitchTo('train-search', appState, setAppState)}>search for trains</div>
    </div>
    </>
  )
};

export default LocationPrompt;
