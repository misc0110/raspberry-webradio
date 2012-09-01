Menu_Info* menu_stations = NULL;

typedef struct {
  char* name;
  char* genre;
  char* url;
  char tag;
} StationInfo;


// ---------------------------------------------------------------------------
void playStation(StationInfo* station) {
  char buffer[512];
  sprintf(buffer, "%s \"%s\" &", Settings_Get("programs", "stream"), station->url);
  system(buffer);
  FILE* f = fopen(Settings_Get("files", "current_station"), "w");
  fprintf(f, "%s\r\n", station->name);
  fclose(f);
}

// ---------------------------------------------------------------------------
ArrayList* readStations() {
  char buffer[256];
  char* ptr;
  
  FILE* f = fopen(STATIONS_FILE, "r");
  if(f == NULL) return NULL;

  ArrayList* stations = AList_Create();
  
  while(fgets(buffer, 256, f) != NULL) {
      if(strlen(buffer) < 3) continue;
      
      StationInfo* info = (StationInfo*)malloc(sizeof(StationInfo));
      ptr = strtok(buffer, "|");
      info->name = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
      strcpy(info->name, ptr);
      ptr = strtok(NULL, "|");
      info->url = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
      strcpy(info->url, ptr);
      ptr = strtok(NULL, "|");
      info->genre = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
      strcpy(info->genre, ptr);
      ptr = strtok(NULL, "|");
      info->tag = '\0';
      if(ptr != NULL) info->tag = ptr[0];
      
      AList_Add(stations, info);
  }
  fclose(f);
  
  return stations;
}

// ---------------------------------------------------------------------------
void playFavorite(int id) {
 ArrayList* stations = readStations();
 
 int i;
 for(i = 0; i < AList_Length(stations); i++) {
   StationInfo* info = (StationInfo*)AList_Get(stations, i);
   if(info->tag == id + '0') {
    playStation(info);
    break;
   }
 }
 
 AList_Destroy(stations);
}

// ---------------------------------------------------------------------------
void init_Stations() {
	// get station list
	menu_stations = Menu_Create(fnt_silkscreen_8, 126, 55);
	Menu_SetAutoIO(menu_stations, 1);
	Menu_SetBorder(menu_stations, BORDER_NONE);
	
	/*
	char buffer[256];
	char* ptr;
	FILE* f = fopen(STATIONS_FILE, "r");
	if(f == NULL) return;
	
	// read stations
	while(fgets(buffer, 256, f) != NULL) {
		if(strlen(buffer) < 3) continue;
		
		StationInfo* info = (StationInfo*)malloc(sizeof(StationInfo));
		ptr = strtok(buffer, "|");
		info->name = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
		strcpy(info->name, ptr);
		ptr = strtok(NULL, "|");
		info->url = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
		strcpy(info->url, ptr);
		ptr = strtok(NULL, "|");
		info->genre = (char*)malloc(sizeof(char) * (1 + strlen(ptr)));
		strcpy(info->genre, ptr);
		ptr = strtok(NULL, "|");
		char tag = '\0';
		if(ptr != NULL) tag = ptr[0];
		// add to station list
		int s_id = Menu_AddItem(menu_stations, info->name);
		Menu_AddItemTag(menu_stations, s_id, (void*)info);
		if(tag != '\0' && tag != '0') Menu_SetTitleTag(menu_stations, s_id, tag);
	}
	fclose(f);
	*/
	
	ArrayList* stations = readStations();
	int i;
	for(i = 0; i < AList_Length(stations); i++) {
	  int s_id = Menu_AddItem(menu_stations, ((StationInfo*)AList_Get(stations, i))->name);
	  Menu_AddItemTag(menu_stations, s_id, AList_Get(stations, i));
	  char tag = ((StationInfo*)AList_Get(stations, i))->tag;
	  if(tag != '\0' && tag != '0') Menu_SetTitleTag(menu_stations, s_id, tag);
	}
	AList_Destroy(stations);
}

// ---------------------------------------------------------------------------
void draw_Stations() {
  Screen_DrawBorder(_lng(STATIONS_TITLE));
  
  Menu_Draw(menu_stations, 1, 8);
}


// ---------------------------------------------------------------------------
void exit_Stations() {
	if(menu_stations != NULL) {
		int i;
		// delete all tags
		for(i = 0; i < Menu_GetItems(menu_stations); i++) {
			StationInfo* info = (StationInfo*)Menu_GetItemTag(menu_stations, i);
			free(info->name);
			free(info->genre);
			free(info->url);
			info->name = NULL;
			info->genre = NULL;
			info->url = NULL;
		}
		Menu_Destroy(menu_stations);
		menu_stations = NULL;
	}
}
