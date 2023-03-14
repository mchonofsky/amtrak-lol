import '../css/search.css'
import { useContext } from 'react';
import { GlobalContext } from '../context/GlobalContext.js'
import { bestTime, makeSwitchTo } from '../context/setup.js'
import ResultBox from './ResultBox.js'
import ResultRow from './ResultRow.js'
import selectTrain from './selectTrain.js'

const today = new Date();

function startOf(k) {
  console.log('start of', k)
  return k.valueOf() - 1000*(k.getHours()*3600 - k.getMinutes()*60 -k.getSeconds()) - k.getMilliseconds();
}

function daysAgo (date) {
  var startOfToday = startOf(today);
  var startOfDate = startOf(date);
  return Math.round((startOfDate - startOfToday)/86400/1000);
}

function ago(time) {
  switch ( daysAgo(time) ) {
    case -5:
      return "five days ago";
    case -4:
      return "four days ago";
    case -3:
      return "three days ago";
    case -2:
      return "two days ago";
    case -1:
      return "yesterday";
    case 0:
      return "";
    case 1:
      return "tomorrow";
    case 2:
      return "in two days";
    case 3:
      return "in three days";
    case 4:
      return "in four days";
    case 5:
      return "in five days";
    default:
      return "";
  }
}

function trainDepartureDetail(first_station) {
  var resultDetails = null;
  const postedTimeAsString = first_station.posted_arrival || first_station.posted_departure;
  if ( postedTimeAsString ) {
    console.log('posteTime')
    const postedTime = new Date(postedTimeAsString);
    console.log(">>>", postedTime, postedTime.getMinutes(), postedTime.getMinutes().toString());
    const minutes = postedTime.getMinutes().toString().padStart(2,'0'); 
    resultDetails = (
      <div class="train-time-box">
        Departed {postedTime.getHours()}:{minutes} {ago(postedTime)}
      </div>
    );
    console.log(resultDetails);
  } else {
    const scheduledTimeAsString 
      = first_station.scheduled_departure || first_station.scheduled_arrival;
    if (scheduledTimeAsString ) {
      console.log('schedTime')
      const scheduledTime = new Date(scheduledTimeAsString);
      const minutes = scheduledTime.getMinutes().toString().padStart(2,'0'); 
      resultDetails = (
        <div class="train-time-box">
          Scheduled {scheduledTime.getHours()}:{minutes} {ago(scheduledTime)}
        </div>
      )
    }
  }
  console.log('final', resultDetails);
  return resultDetails;
}

function MultipleTrains( props ) {
  const [appState, setAppState ] = useContext(GlobalContext);
  const content = appState.trains.map( (train) => {
    const onClick = selectTrain(train.train_id, appState, setAppState);
    const label = `${train.first_station_name}, ${train.first_station_state_code} to ${train.last_station_name}, ${train.last_station_state_code}`;
    
    const first_station_opts = train.details.filter( (s) => s.station_id === train.first_station_station_id);
    
    /* posted if posted
     * otherwise scheduled */
    
    var first_station = null;
    if ( first_station_opts.length > 0 ) {
      first_station = first_station_opts[0];
    } else {
      first_station = train.details.sort( (a,b) => bestTime(a) - bestTime(b) )[0];
    }
    const resultDetail = trainDepartureDetail(first_station);
    console.log('resultDetail', resultDetail);
    return (
      <ResultRow 
        onClickPassThrough={onClick} 
        resultLabel={label} 
        resultDetails={resultDetail} />
    );
  });

  return (
  <>
    <div class="search">
      Which train is it?
    </div>
      <ResultBox addClass='no-search' content={content}/>
    <div class='morebutton' onClick={makeSwitchTo('train-search', appState, setAppState)}>search for more trains
    </div>
  </>
  );
};

export default MultipleTrains;
