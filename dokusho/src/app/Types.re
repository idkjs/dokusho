module Routes {
  type route =
  | Home(string)
  | NoAuth;
};

type pageType =
  | Manga
  | News
  | Book
  | Lyric
  | Net;

type content = {
  name: string,
  pageType
};

type entry = {
  id: int,
  kind: pageType,
  value: int
};

type day = {
  date: string,
  entries: list(entry)
};

type readingHistory = {days: list(day)};

type mainState = {
  readingData: readingHistory,
  selectedEntry: pageType,
  selectedDate: Js.Date.t
};

type action =
  | AddEntry(pageType, int)
  | ChangeSelection(pageType)
  | UpdateHistory(list(day))
  | LoadUserData(string)
  | SelectDate(Js.Date.t);

type authData = { 
  accessToken: string,
  idToken: string,
  expiresIn: string
};

module Decoders = {
  let parseAuthData = (json: Js.Json.t) : authData =>
    Json.Decode.{
      accessToken: json |> field("accessToken", string),
      idToken: json |> field("idToken", string),
      expiresIn: json |> field("expiresIn", string)
    };
  let parsePageType = (asString:string) => {
    switch (asString) {
    | "Manga" => Manga
    | "News" => News
    | "Book" => Book
    | "Lyric" => Lyric
    | "Net" => Net
    | _ => Book
    };
  };
  let parseEntry = (json: Js.Json.t) : entry =>
    Json.Decode.{
      id: json |> field("id", int),
      kind: json |> field("kind", string) |> parsePageType,
      value: json |> field("value", int)
    };
  let parseEntriesJson = json => Json.Decode.list(parseEntry, json);
  let parseDay = (json: Js.Json.t) : day =>
    Json.Decode.{
      date: json |> field("date", string),
      entries: json |> field("entries", parseEntriesJson)
    };
  let parseDaysJson = json => Json.Decode.list(parseDay, json);
  let parseHistory = (json: Js.Json.t) : readingHistory =>
    Json.Decode.{days: json |> field("days", parseDaysJson)};
};

module Encoders = {
  let encodeAuthData = authData =>
    Json.Encode.(
      object_([
        ("accessToken", string(authData.accessToken)),
        ("idToken", string(authData.idToken)),
        ("expiresIn", string(authData.expiresIn))
      ])
    );
  let encodeEntry = entry =>
    Json.Encode.(
      object_([
        ("id", int(entry.id)),
        (
          "kind",
          string(
            switch entry.kind {
            | Manga => "Manga"
            | News => "News"
            | Book => "Book"
            | Lyric => "Lyric"
            | Net => "Net"
            }
          )
        ),
        ("value", int(entry.value))
      ])
    );
  let encodeDay = day =>
    Json.Encode.(
      object_([
        ("date", string(day.date)),
        ("entries", day.entries |> list(encodeEntry))
      ])
    );
  let encodeReadingHistory = readingHistory =>
    Json.Encode.(object_([("days", readingHistory.days |> list(encodeDay))]));
  let endcodeInput = (pageType, value) =>
    Json.Encode.(
      object_([
        (
          "kind",
          string(
            switch pageType {
            | Manga => "Manga"
            | News => "News"
            | Book => "Book"
            | Lyric => "Lyric"
            | Net => "Net"
            }
          )
        ),
        ("value", int(value))
      ])
    );
  let endcodeStringInput = (pt, value) =>
    Json.Encode.(
      object_([
        ("kind", string(pt)),
        ("value", int(value))
      ])
    );
};
