import { useContext } from 'react';
import {GlobalContext } from '../context/GlobalContext.js'
import {makeSwitchTo } from '../context/setup.js'
import ResultBox from './ResultBox.js';
import ResultRow from './ResultRow.js';

function formatTime(timeString) {
  let time = new Date(Date.parse(timeString));
  return time.getHours().toString().padStart(2,'0')+':'+time.getMinutes().toString().padStart(2,'0') + 
    ((time.getHours() > 11) ? ' am' : ' pm');
}

function TrainDetails(props) {
  const [appState, setAppState ] = useContext(GlobalContext);
  let stations = appState.selectedTrainDetails.stations
    .sort((a,b) => Date.parse(a.scheduled_arrival || a.scheduled_departure) 
      - Date.parse(b.scheduled_arrival || b.scheduled_departure ) );
  console.log('>>> sTD', appState.selectedTrainDetails);
  const content = 
    stations.map(station => {
      console.log(station.station_id, appState.selectedTrainDetails.first_station.station_id);
      const detail = (
        <>
          <div class="train-time-box">
            { formatTime( station.scheduled_arrival || station.scheduled_departure )}
          </div>
          { (station.posted_arrival || station.posted_departure) && 
            <div class="train-time-box">
              { ((station.posted_departure && (station.station_id !== appState.selectedTrainDetails.last_station.station_id)) &&
                <>
                  Departed {formatTime(station.posted_departure)}
                </> )
                || 
                (station.posted_arrival &&
                <>
                  Arrived {formatTime(station.posted_arrival)}
                </>
                )
              }
            </div>
          }
        </>
      );
      return <ResultRow 
      onClickPassThrough={() => console.log('clicked')}
      resultLabel={station.display_name}
      resultDetails={detail}
      />
    });

  return (
  <>
    <ResultBox addClass="station-details" content={content} />
    <div class='morebutton' onClick={makeSwitchTo('time-display', appState, setAppState)}>
    back to your train
    </div>
  </>
  );
};


export default TrainDetails;
