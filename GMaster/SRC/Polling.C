  switch(QueryState) {
  case IDLE_QUERY:
    break;
  case WAIT_QUERY:
    // over time
    if (bOverTimeQuery) {
      if (QueryCount++ < MAX_QUERY) {
        QueryState = MAIN_QUERY;
      } else {
        QueryState = STOP_QUERY;
      }
    }
    break;
  case INIT_QUERY:
    P1Stop(); // stop P1 system
    P2Stop(); // stop P2 system
    QueryCount = 0;
    bOverTimeQuery = 0;
    QueryState = WAIT_QUERY;
    break;
  case MAIN_QUERY:
  // MainStation LED Status // return GREEN, GROUP, FIRE, CONTROL
    if (QueryCount < MAX_QUERY) {
      if (IsQuery(QueryCount-1)) {
        // Query == 1 QueryMain, 2 ~ 17 is extern unit
        P1QueryState(QueryCount-1);
        bOverTimeQuery = 0;
      }
    }
    QueryState = WAIT_QUERY;
    break;
  case STOP_QUERY:
    P1Run();
    P2Run();
    break;
  }