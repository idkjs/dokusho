open Input;
open Entry;
open Types;
open Day;
open PageTypeSelection;

module Dokusho {
  let component = ReasonReact.reducerComponent("Dokusho");
  let initState = () => {
    readingData: { days : [Day.now()] },
    selectedEntry: Book
  };
  
  let make = (_children) => {
    ...component,
    initialState: () => initState(),
    reducer: (action, { readingData, selectedEntry }) => 
      switch action {
        | ChangeSelection(pageType) => 
            ReasonReact.Update({readingData: readingData, selectedEntry: pageType}); 
        | AddEntry(pageTypeString, count) =>  
            ReasonReact.Update(Day.createEntry(9, pageTypeString, count) 
              |> Day.appendEntry(Day.now()) 
              |> (ne) => List.map(d => if(d.date != ne.date) d 
                  else { 
                    date: d.date, 
                    entries: List.rev_append(ne.entries, d.entries)}
                    , readingData.days)
              |> (ds) => { days: ds }
              |> (rd) => { readingData: rd, selectedEntry: selectedEntry });
      },
    render: (self) => {
      let pageCount = Day.pageCount(List.hd(self.state.readingData.days));      
      
      <div className="app">
        <div className="title"> 
          (ReasonReact.stringToElement("Dokusho"))
          <Input 
            selection=self.state.selectedEntry
            onSubmit=(self.reduce((text) => AddEntry(self.state.selectedEntry, int_of_string(text))))
            />
          <PageTypeSelection onChangeSelect=(self.reduce(selected => ChangeSelection(selected))) />
        </div>
        <div className="entries">
          (List.hd(self.state.readingData.days).entries
              |> List.map((entry: entry) => <Entry key=(string_of_int(entry.id)) entry=entry />) 
              |> Array.of_list
              |> ReasonReact.arrayToElement)
        </div>
        <div className="footer">
          (ReasonReact.stringToElement(string_of_float(pageCount) ++ " pages"))
        </div>
      </div>
    }
  };
  
}