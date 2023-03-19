import '../css/search.css'

function ResultRow ( {onClickPassThrough, resultLabel, resultDetails} ) {
  return (
    <div class="result" onClick={onClickPassThrough}>
      <div class="result-label">
        {resultLabel}
      </div>
    {resultDetails &&
      <>
        <br/>
        {resultDetails} 
      </>
    }
    </div>
  );
}

export default ResultRow;
