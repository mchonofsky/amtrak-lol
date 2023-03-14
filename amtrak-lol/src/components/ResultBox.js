function ResultBox({content, addClass} ) {

  return (
    <>
      <div class={'resultbox ' + (addClass || " " )}>
        { content }
      </div>
    </>
  );
}

export default ResultBox;
