import { useContext } from 'react';
import {GlobalContext } from '../context/GlobalContext.js'
import ResultBox from './ResultBox.js';
import SearchResultRow from './SearchResultRow.js';

function SearchResultBox( ) {
  const [appState, ] = useContext ( GlobalContext);
  
  var trainRows = appState.trains.map( (train) => {
    return (
      <SearchResultRow train={train} />
    )
  });

  return (
    <ResultBox addClass='' content={trainRows}/>
  );
}

export default SearchResultBox;
