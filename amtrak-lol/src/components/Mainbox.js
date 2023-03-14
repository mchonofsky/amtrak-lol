import React, { useContext } from 'react';
import LocationPrompt from './location_prompt.js'
import TimeDisplay from './time_display.js'
import TrainDetails from './train_details.js'
import TrainSearch from './train_search.js'
import MultipleTrains from './multiple_trains.js'
import {GlobalContext } from '../context/GlobalContext.js';

function Mainbox() {
  const [appState, ] = useContext(GlobalContext );
  // appState.currentDisplay = {}
  //
  // location-prompt
  // train-search
  // time-display
  // more-details
  
  return (
      <div class="mainbox">
        { appState.currentDisplay === 'location-prompt' && (
          <LocationPrompt />
        )}
        { appState.currentDisplay === 'multiple-trains' && (
          <MultipleTrains />
        )}
        { appState.currentDisplay === 'time-display' && (
          <TimeDisplay />
        )}
        { appState.currentDisplay === 'train-details' && (
          <TrainDetails />
        )}
        { appState.currentDisplay === 'train-search' && (
          <TrainSearch />
        )}
      </div>
  );
}

export default Mainbox;
