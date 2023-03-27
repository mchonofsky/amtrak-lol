import React, { useState, useContext } from 'react';                                                             
import {GlobalContext } from '../context/GlobalContext.js'
import selectTrain  from './selectTrain.js'
import axios from 'axios';
import {makeSwitchTo, switchTo} from '../context/setup.js';
import '../css/spinner.css'

function LocationPrompt(props) {
  const [appState, setAppState ] = useContext(GlobalContext );
  
  const [errorPromptVisible, setErrorPromptVisible ] = useState(false);
  const [spinnerVisible, setSpinnerVisibility ] = useState(false);

  const errorPromptInvisible = () => {
    setErrorPromptVisible(false);
  }

  const getLocation = () => {
    setSpinnerVisibility(true);
    const options = { enableHighAccuracy: true, timeout: 15000};
    const doError = (err) => 
      {
        setSpinnerVisibility(false);
        console.log(err);
        setErrorPromptVisible(true);
        //setTimeout(errorPromptInvisible, 3000);
      };
    const doSuccess = async (result) => {
      console.log('in doSuccess');
      // gets geolocation
      console.log('location is', result);
      appState.geolocation = result;
      setAppState({...appState, 'geolocation': result})
      try {
        let response = await axios.post('/api/get_train', {
          coords: {
            'latitude': appState.geolocation.coords.latitude, 
            'longitude': appState.geolocation.coords.longitude, 
            'speed': appState.geolocation.coords.speed
          },
          timestamp: appState.geolocation.timestamp
        })
        // then retrieves train
        appState.trains = response.data;
        setAppState({...appState, 'trains': response.data});
        if ( appState.trains.length > 1 ) {
          switchTo('multiple-trains', appState, setAppState);
        } else if ( appState.trains.length === 1 ) {
          selectTrain(appState.trains[0].train_id, appState, setAppState)();
          //switchTo('time-display', appState, setAppState);
        }
        setSpinnerVisibility(false);
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
    <div class="mainbox-box">
      <div class="morebutton trains" onClick={makeSwitchTo('train-search', appState, setAppState)}>search for trains</div>
    </div>
    </div>
    { spinnerVisible && (
      <div class="lds-dual-ring"></div>
      )
    }
    </>
  )
};

export default LocationPrompt;
