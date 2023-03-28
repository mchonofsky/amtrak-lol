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

  const getCurrentStations = () => {
    // returns {next_station: xxx, last_station: xxx}
    var train = appState.selectedTrainDetails; 
    // remove any missing data
    train.stations = train.stations.filter( (a) => Date.parse(a.scheduled_departure || a.scheduled_arrival) ) 
    // sort on arrival time
    train.stations = train.stations.sort((a,b) => {
        return Date.parse(b.scheduled_departure || b.scheduled_arrival ) > Date.parse( a.scheduled_departure || a.scheduled_arrival )
          ? -1 : 1;
        
      }
    )
    console.log('ts', train.stations);
    console.log(train.stations);
    for (var i = 0; i < train.stations.length; i++) {
      if (train.stations[i].posted_arrival === null && train.stations[i].posted_departure === null) {
        return {next_station: train.stations[i], last_station: train.stations[i-1], allComplete: false}
      }
    }
    return {next_station: undefined, last_station: train.stations[train.stations.length], allComplete: true}
  }

  var currentStations = getCurrentStations();
  console.log('cerruntStations',currentStations)
  var nextStation = currentStations.next_station;
  console.log('nextStation', nextStation)
  //var lastStation = train.stations.reduce((accumulator, currentValue) => ) 
  var lastStopArrivedLate = false;
  var lastStopDepartedLate = false
  if (currentStations.last_station && (Date.parse(currentStations.last_station.scheduled_arrival) < Date.parse(currentStations.last_station.posted_arrival))) {
    lastStopArrivedLate = true
  }
  if (currentStations.last_station && (Date.parse(currentStations.last_station.scheduled_departure) < Date.parse(currentStations.last_station.posted_departure))) {
    lastStopDepartedLate = true
    var lastStopMinutesLate= Math.round((Date.parse(currentStations.last_station.posted_departure) - Date.parse(currentStations.last_station.scheduled_departure))/60000);
  }
  return (
    <>
      <div class="mainbox-box first">
        <div class="top-header">
        { currentStations.allComplete && (
          <>looks like you've arrived at {train.last_station.display_name}</>      
        )}
        { (! currentStations.allComplete ) && (
          <>next stop <span class="highlight">{nextStation.display_name}</span> in <span class="highlight">{Math.round((Date.parse(
            currentStations.next_station.scheduled_arrival || currentStations.next_station.scheduled_departure
          ) - current_time)/60000) }</span> minutes</>
        )}
        { ( lastStopDepartedLate) && 
        <div class="supporting-text">
            {lastStopMinutesLate} {lastStopMinutesLate === 1 ? "minute" : "minutes"} late at the last stop
        </div>
        }
      </div>
      </div>
      <div class="mainbox-box">
          <div class="supporting-text">
            { (! currentStations.allComplete) && ( <> looks like you're on train {train.train_data.amtrak_train_number} to {train.last_station.display_name}</> ) }
          <div class="morebutton details" onClick={makeSwitchTo('train-details', appState, setAppState)} >more stops</div>
            { (currentStations.allComplete) && ( <> thanks for using amtrak.lol. tell your friends </> ) }
          
          </div>
      <div class="morebutton trains" onClick={makeSwitchTo('multiple-trains', appState, setAppState)} >other trains</div>
      </div>
    </>
  )
}

export default TimeDisplay;

