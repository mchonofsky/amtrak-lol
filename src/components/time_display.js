import '../index.css'
import React, { useContext } from 'react';
import {GlobalContext, getCurrentTrain } from '../context/GlobalContext.js';
import {bestTime, ascendingSort, stDistance, makeSwitchTo} from '../context/setup.js';

const DEBUG_MODE = true;

function TimeDisplay(props) {
  const [appState, setAppState ] = useContext( GlobalContext );

  var train = getCurrentTrain(appState);
  console.log('appState', appState);
  console.log('train is', train)
  train.stations.sort( (s,t) => ascendingSort(bestTime(s), bestTime(t)));
  console.log('appState', appState)
  
  var current_time = new Date();
  if ( DEBUG_MODE ) { 
    current_time = Date.parse(train.current_time);
  }   
  const getNextStation = () => {
    // note the reverse in the for loop
    var stations_to_consider = [];
    for (var i = 0; i < train.stations.reverse().length; i++) {
      if (train.stations[i].posted_arrival || train.stations[i].posted_departure) {
        break;
      }
      stations_to_consider.push(train.stations[i]);
    }
    // reverse rectified
    train.stations.reverse();
    stations_to_consider.reverse();
    
    if (stations_to_consider.length === 0 )
    {
      return {allComplete: true};
    }
    console.log('stations to consider', stations_to_consider);
    return stations_to_consider.reduce( (nextStationObj, station) => {
      /* get the nearest station that we haven't already been to */
      /* accumulate a [distance, station] array */
      if (
        (! (station.posted_arrival || station.posted_departure) )  
          && 
        (stDistance(train.latitude, train.longitude, station.latitude, station.longitude) < nextStationObj.distance ))
        {
          return { 
            distance: stDistance(train.latitude, train.longitude, station.latitude, station.longitude),
            station: station
          };
        } else {
          return nextStationObj;
        }
    }
    , {allComplete: false, distance: 99999, station: {} });
  }

  var nextStationIdentified = getNextStation();
  var nextStation = nextStationIdentified.allComplete ? nextStationIdentified.station : {};
  
  return (
    <>
      <div class="mainbox-box first">
      { nextStationIdentified.allComplete && (
        <div class="top-header">looks like you've arrived at {train.last_station.display_name}</div>      
      )}
      { (! nextStationIdentified.allComplete ) && (
        <>
          <div class="top-header">next stop <span class="highlight">{nextStation.display_name}</span> in <span class="highlight">{Math.round((Date.parse(bestTime(nextStation)) - current_time)/60000) }</span> minutes</div>
          <div>you were 32 minutes late at the last stop</div>
        </>
      )}
        </div>
          <div class="mainbox-box">
          
          <div class="supporting-text">
            { (! nextStationIdentified.allComplete) && ( <> looks like you're on train {train.train_data.amtrak_train_number} to {train.last_station.display_name}</> ) }
          <div class="morebutton details" onClick={makeSwitchTo('train-details', appState, setAppState)} >more stops</div>
            { (nextStationIdentified.allComplete) && ( <> thanks for using amtrak.lol. tell your friends </> ) }
          
          </div>
          <div class="morebutton trains" onClick={makeSwitchTo('multiple-trains', appState, setAppState)} >more trains</div>
      </div>
    </>
  )
}

export default TimeDisplay;

