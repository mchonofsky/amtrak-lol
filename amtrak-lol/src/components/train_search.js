import {useState, useContext } from 'react';
import '../css/search.css'
import '../css/autocomplete.css'
import { GlobalContext} from '../context/GlobalContext.js'
import SearchResultBox from './SearchResultBox.js'
import axios from 'axios';
import {SearchContext, SearchValueContext, SuggestionContext} from '../context/Contexts.js'


async function refreshResults(searchTags, appState, setAppState) {
  if (searchTags.length === 0 )
  {
    console.log('no SearchTags')
    appState.trains = [];
    setAppState({...appState, trains:[]});
    console.log(appState.trains);
    return;
  }
  try {
    console.log('searchTags is', searchTags)
    let post_result = await axios.post('/api/search_trains', {station_codes: searchTags.map( st => st.station_id )});
    appState.trains = post_result.data;
    setAppState({...appState, trains: post_result.data})
  } catch {
    console.log('could not post');
    return;
  }
}

function removeTag(station_id, searchTags, setSearchTags, appState, setAppState) {
  return () => {
    console.log('in removeTag, removing', station_id, searchTags.map ( tag => tag.station_id));
    let tags = searchTags.filter ( tag => tag.station_id !== station_id );
    setSearchTags([...tags]);
    refreshResults(tags, appState, setAppState);
    console.log('current tags', searchTags.map(tag => tag.station_id))
  }
}

function SearchTag(props ) {
  console.log('SearchTag props', props);
  return (
          <span class="search-tag">
            <span class="search-item">{props.tag}</span><span class="close-button" onClick={props.removeFunction}>x</span>
          </span>
    )
}

function SearchTags(props ) {
  const [appState, setAppState] = useContext( GlobalContext);
  const [searchTags, setSearchTags] = useContext( SearchContext);
  
  let searchTagItems = searchTags.map( (item) =>
    <SearchTag tag={item.station_id} removeFunction={ removeTag(item.station_id, searchTags, setSearchTags, appState, setAppState)} />
  );
  if ( searchTagItems.length > 0 ) {
    return (
        <div class="search-tags">
          {searchTagItems}
        </div>
    );
  }
}

function SearchSuggestion({suggestion, onClickPassThrough } ){
  return (
    <>
      <li onClick={onClickPassThrough}>{suggestion.search_display}</li>
    </>
  );
}

function SearchSuggestions(props ) {
  const [appState, setAppState] = useContext(GlobalContext );
  const [suggestionItems, setSuggestionItems] = useContext(SuggestionContext);
  const [searchTags, setSearchTags ] = useContext(SearchContext);
  const [, setSearchValue ] = useContext(SearchValueContext);

  let suggestionComponents = suggestionItems.map( (suggestion) =>
    <SearchSuggestion suggestion={suggestion} onClickPassThrough={ () => {
        // copied from addTag, below. maybe we could consolidate
      if (suggestion.station_code in searchTags.map(st => st.station_code) )
          {
            console.log('in 1')
          }
        else
          {
            console.log('in 2')
            searchTags.push(suggestion);
            setSearchTags([...searchTags]);
          }
        setSuggestionItems([]);
        refreshResults(searchTags, appState, setAppState);
        setSearchValue('');
        return;
      }
    }/>
  )

  return (
    <>
      { (suggestionComponents.length > 0) &&
        (
          <div class="result-ac">
            <ul class="sugg-list">
              { suggestionComponents }
            </ul>
          </div>
        )
      }
    </>
  )
}

function TrainSearch( props ) {
  const [appState, setAppState] = useContext( GlobalContext);
  //searchTags is code: full suggestion
  const [searchTags, setSearchTags] = useState([]);
  const [suggestionItems, setSuggestionItems] = useState([]);
  const [searchValue, setSearchValue ] = useState('');


  function removeAllSuggestions() {
    suggestionItems.length = 0; // we have references we need to preserve
  }

  function getDistinctSuggestions(event) {
    let value = event.target.value.trim().toLowerCase();
    let targets = appState.searchStations.filter( suggestion => suggestion.search_target.includes(value))
    var distinct_suggestions = [];
    const map = new Map();
    for (const item of targets) {
      /* above, we filter to only those that include the search target, eg WAS and Washington, DC */
      /* here, we use search_display in order to ensure that the user experience includes only one entry for
       * each station. */
        if(!map.has(item.search_display)){
            map.set(item.search_display, true);    // set any value to Map
            distinct_suggestions.push(item);
        }
    }
    var starts = (x,y) => x.search_display.toLowerCase().startsWith(y)
    distinct_suggestions.sort((t1, t2) => {
        if (t1.station_id.toLowerCase() === value) return -3;
        else if ( starts(t1, value) && (! starts(t2, value))) return -1;
        else if ( starts(t2, value) && (! starts(t1, value))) return 1;
        else return t1 > t2 ? -2 : 2;
      }
    );
    console.log(distinct_suggestions);
    return distinct_suggestions;
  }

  function handleKeyUp(event) {
    console.log('event is', event);
    event.stopPropagation();
    //event.preventDefault();
    removeAllSuggestions();
    if (event.target.value.length > 2)
      setSuggestionItems(getDistinctSuggestions(event));
  }

  function addTag(suggestion) {
    if (! suggestion) return;
    if (suggestion.station_id in searchTags.map(st => st.station_id) )
      return;
    setSearchTags([...searchTags, suggestion]);
    refreshResults(searchTags, appState, setAppState);
  }

  function handleSubmit(event) {
    event.stopPropagation();
    event.preventDefault();
    event.target.value = '';
    addTag(suggestionItems[0]);
    setSuggestionItems([]);
    setSearchValue('');
  }

  return (
  <>
    <div class="search">
      <SearchContext.Provider value={[searchTags, setSearchTags]}>
        <SearchValueContext.Provider value={[searchValue, setSearchValue]}>
        <div class="searchbox">
          <form autocomplete="off" onSubmit={handleSubmit}>
            <input value={searchValue} type="text" placeholder="pick a station" onInput={e => setSearchValue(e.target.value)} onKeyUp={handleKeyUp} />
          </form>
          <SuggestionContext.Provider value={[suggestionItems, setSuggestionItems]} >
            <SearchSuggestions />
          </SuggestionContext.Provider >
        </div>
        <SearchTags />
      </ SearchValueContext.Provider >
      <SearchResultBox />
      </ SearchContext.Provider >
    </div>
  </>
  );
};

export default TrainSearch;
