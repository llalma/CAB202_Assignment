#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>
#include <stdio.h>
#include <windows.h>

//Global Variables
	bool GameOver = false;
	bool NewGame = true;
	
	//LHS and RHS road coordinates 
	int LHSRoad;
	int RHSRoad;
	
	double CurrentSpeed = 0;
	
	// Condition of car
	int Condition = 100;
	
	//Distance
	int DistanceTravelled = 0;
	
	//FuelRemaining
	double FuelRemaining = 100;
	
	// How long vehicle has stopped at gas station
	double StopTimeGas = 0;
	
//Distance required to reach zombie mountain
	#define Winning_Distance (10000)

//Sprites
	//RaceCar
		char * RaceCar_image =
		/**/  "- -"
		/**/  " | "
		/**/  " | "
		/**/  "- -";
	
		#define widthRaceCar (3)
		#define heightRaceCar (4)
		
		//	Declare a sprite_id
		sprite_id RaceCar;
		
	//Road Lines
		char * RoadLines_image =
		/**/  "|"
		/**/  "|";
	
		#define widthRoadLines (1)
		#define heightRoadLines (2)
		

		//	Declare a sprite_id
		#define Max_RoadLines (250)
		sprite_id RoadLines[Max_RoadLines];
	
	//Gas Station
		char * GasStation_image =
		/**/  "_________"
		/**/  " |     | "
		/**/  " | GAS | "
		/**/  " |     | ";
		
		#define widthGasStation (9)
		#define heightGasStation (4)
		
		//	Declare a sprite_id
		sprite_id GasStation;
		
	//Tree
		char * Tree_image =
		/**/  "  88  "
		/**/  "888888"
		/**/  "  ||  "
		/**/  "  ||  "
		/**/  "  ||  ";
		
		#define widthTree (6)
		#define heightTree (5)
		
		//	Declare a sprite_id
		#define Max_Trees (100)
		sprite_id Tree[Max_Trees];
		
	//Bush
		char * Bush_image =
		/**/  "8888"
		/**/  "8888";
	
		#define widthBush (4)
		#define heightBush (2)
		
		//	Declare a sprite_id
		#define Max_Bushs (50)
		sprite_id Bush[Max_Bushs];
	
	//WreckedCar
		char * OldCars_image =
		/**/  "O-------O"
		/**/  "|       |"
		/**/  "|       |"
		/**/  "O-------O";
	
		#define widthOldCars (9)
		#define heightOldCars (4)
		

		//	Declare a sprite_id
		#define Max_OldCars (10)
		sprite_id OldCars[Max_OldCars];
		
	
	//Zombie
		char * Zombie_image =
		/**/  " O "
		/**/  "-|-"
		/**/  "/ \\";
	
		#define widthZombie (3)
		#define heightZombie (3)
		

		//	Declare a sprite_id
		#define Max_Zombies (100)
		sprite_id Zombie[Max_Zombies];
		
	//Ai Car
		char * AICar_image =
		/**/  "0 0"
		/**/  " | "
		/**/  " | "
		/**/  "0 0";
		
		#define widthAICar (3)
		#define heightAICar (4)
		
		//	Declare a sprite_id
		#define Max_AICars (50)
		sprite_id AICars[Max_AICars];
		
		// Speed of each AICar_Count
		double CurrentSpeedAI[Max_AICars];
		
		//Record if an AI car has crashed
		bool CrashAI[Max_AICars];
		
		//Used to change the height of the border between the playing area and the dashboard. Is used to hide sprites when they first move
		#define TallestObject_minus_2 (3)
		
		//Total number of sprites
		#define NumberOfSprites (200)
		
// Setup game screen
	void Setup(void) {
		// Clears the screen of anything previously displayed
		clear_screen();
		// X & Y positions of race car initially, minus height of racecar to ensure whole car is on screen.
		double x = (screen_width()/2)-1;
		double y = (screen_height()/4)*3.5-heightRaceCar;
		
		RaceCar = sprite_create(x,y,widthRaceCar,heightRaceCar,RaceCar_image);
		sprite_draw(RaceCar);	
	}
//Collisions 	
	//Check if Car collides with an item of scenery
		bool CollidedScenery(sprite_id scenery){
			int RaceCarLeft = round(sprite_x(RaceCar));
			int RaceCarTop = round(sprite_y(RaceCar));
			int RaceCarRight = RaceCarLeft + widthRaceCar-1;
			int RaceCarBottom = RaceCarTop + heightRaceCar-1;
			
			int SceneryLeft = round(sprite_x(scenery));
			int SceneryTop = round(sprite_y(scenery));
			int SceneryRight = SceneryLeft + sprite_width(scenery)-1;
			int SceneryBottom = SceneryTop + sprite_height(scenery)-1;
				
			if(SceneryTop>RaceCarBottom) return false;
			if(RaceCarTop>SceneryBottom) return false;
			if(RaceCarLeft>SceneryRight) return false;
			if(SceneryLeft>RaceCarRight) return false;
			
			Condition = Condition - CurrentSpeed*10;
			if(Condition <= 0){
				return true;
			}else{
				// X & Y positions of race car initially, minus height of racecar to ensure whole car is on screen.
				sprite_move_to(RaceCar,(screen_width()/2)-1,(screen_height()/4)*3.5-heightRaceCar);
				CurrentSpeed = 0;
				FuelRemaining = 100;
				return false;
			}
		}
				
	//Check car does not spawn ontop of an Object after a collision, if so then remove the object
		bool Check_Location(sprite_id Object){
			int RaceCarLeft = round(sprite_x(RaceCar));
			int RaceCarTop = round(sprite_y(RaceCar));
			int RaceCarRight = RaceCarLeft + widthRaceCar-1;
			int RaceCarBottom = RaceCarTop + heightRaceCar-1;
			
			int ObjectLeft = round(sprite_x(Object));
			int ObjectTop = round(sprite_y(Object));
			int ObjectRight = ObjectLeft + sprite_width(Object)-1;
			int ObjectBottom = ObjectTop + sprite_height(Object)-1;
			
			if(ObjectTop>RaceCarBottom) return false ;
			if(RaceCarTop>ObjectBottom) return false ;
			if(RaceCarLeft>ObjectRight) return false ;
			if(ObjectLeft>RaceCarRight) return false ;
			
			sprite_move_to(Object,screen_width()/2-1,-5);
			return true;
		}

//Borders
	void Border(int left, int right, int top, int bottom, char borderObject) {
		
		//Horizontal Lines
		draw_line(left,top,right,top,borderObject);
		draw_line(left,bottom,right,bottom,borderObject);
		
		//Vertical Lines
		draw_line(left,top,left,bottom,borderObject);
		draw_line(right,top,right,bottom,borderObject);		
	}
	
//Dashboard	
	//Dashboard Titles
		void draw_Dashboard_Titles(int Mode){
			int divider;
			if(Mode == 1){
				divider = 12;
				draw_string((11*screen_width()-2)/divider-4,screen_height()/20,"Position");
			}else{
				divider = 10;
			}
			draw_string((screen_width()-3)/divider-2,screen_height()/20,"Time");
			draw_string(3*(screen_width()-3)/divider-4,screen_height()/20,"Distance");
			draw_string(5*(screen_width()-3)/divider-4,screen_height()/20,"Condition");
			draw_string(7*(screen_width()-1)/divider-4,screen_height()/20,"Fuel");
			draw_string(9*(screen_width()-3)/divider-3,screen_height()/20,"Speed");
		}
		
	//Dashboard Values
		void draw_Dashboard_Values(double DisplayTime, int Mode){
			int divider;
			if(Mode == 1){
				divider = 12;
			}else{
				divider = 10;
			}
			draw_double((screen_width()-3)/divider-2,2*screen_height()/20+1,DisplayTime);
			draw_int(3*(screen_width()-3)/divider-1,2*screen_height()/20+1,DistanceTravelled);
			draw_int(5*(screen_width()-3)/divider-1,2*screen_height()/20+1,Condition);
			draw_double(7*(screen_width()-3)/divider-1,2*screen_height()/20+1,round(FuelRemaining));
			draw_int(9*(screen_width()-3)/divider-1,2*screen_height()/20+1,CurrentSpeed);
		}
		
	//Timer
	double Timer(double startTime){
		double currentTime = get_current_time();
		double displayTime = round(100*(currentTime - startTime))/100;
		return displayTime;
	}
	
	//Compile Dashboard methods
	void DashBoard(double startTime, int Mode){
		//Timer
		double DisplayTime = Timer(startTime);
		
		//Distance Travelled
		DistanceTravelled += CurrentSpeed;
		
		//Fuel Remaining, only if survival mode is selected
		if(Mode != 1){
			FuelRemaining = FuelRemaining - (CurrentSpeed/10*(Winning_Distance/(screen_height()*100)))/2;
			if(FuelRemaining <= 0){
				GameOver = true;
			}
		}
		
		//Draw Border seperation between dashboard and playing area
		for(int i = 0; i <= TallestObject_minus_2; i++){
			Border(1,screen_width()-2,screen_height()/5+i,screen_height()/5+i,'+');
		}
		
		// Seperation of dashboard depending on the game mode.		
		if(Mode == 1){
			int Sections5[] = {(screen_width()-3)/6,2*(screen_width()-3)/6,3*(screen_width()-3)/6,4*(screen_width()-3)/6,5*(screen_width()-3)/6};
			for(int i = 0; i <= (sizeof(Sections5)/sizeof(int))-1;i++){
					Border(Sections5[i],Sections5[i],1,screen_height()/5,'+');
			}
		}else{
			int Sections4[] = {(screen_width()-3)/5,2*(screen_width()-3)/5,3*(screen_width()-3)/5,4*(screen_width()-3)/5};
			for(int i = 0; i <= (sizeof(Sections4)/sizeof(int))-1;i++){
					Border(Sections4[i],Sections4[i],1,screen_height()/5,'+');
			}
		}
	
		//Draw Titles
		draw_Dashboard_Titles(Mode);
		
		//Draw Values
		draw_Dashboard_Values(DisplayTime,Mode);
	}
		
//Environments
	//Road
		void draw_Roads(void){
			LHSRoad = screen_width()/2 - screen_width()*0.2;
			RHSRoad = screen_width()/2 + screen_width()*0.2;
				
			draw_line(LHSRoad,screen_height()/5+TallestObject_minus_2,LHSRoad,screen_height()-1,'|');
			draw_line(RHSRoad,screen_height()/5+TallestObject_minus_2,RHSRoad,screen_height()-1,'|');
		}
		
	//initialize Road Lines
		void initialize_RoadLines(int RoadLines_Count){
			
			for(int i = 0; i<=RoadLines_Count; i++){
				// X_Coordinates
				int X_Coordinates = screen_width()/2-1;						
				//Y_Coordinates
				int Y_Coordinates = screen_height()/5 + i*4;
				
				RoadLines[i] = sprite_create(X_Coordinates,Y_Coordinates,widthRoadLines,heightRoadLines,RoadLines_image);
				if(Y_Coordinates>=screen_height()/5){
					sprite_draw(RoadLines[i]);
				}
			}
		}
		
	//initialize Tree
		void initialize_Trees(int Tree_Count){
			
			for(int i = 0; i<=Tree_Count; i++){
				// X_Coordinates
				int X_Coordinates;
					if(rand()%2 == 1){
						X_Coordinates = rand()%LHSRoad-widthTree+1;
					}else{
						X_Coordinates = (rand()%LHSRoad)+RHSRoad;
					}
							
				//Y_Coordinates
				int Y_Coordinates = (rand()%screen_height())-(screen_height()/2);
				
				Tree[i] = sprite_create(X_Coordinates,Y_Coordinates,widthTree,heightTree,Tree_image);
				if(Y_Coordinates>=screen_height()/5){
					sprite_draw(Tree[i]);
				}
			}
		}
		
	//initialize Bush
		void initialize_Bush(int Bush_Count){
			for(int i = 0; i<=Bush_Count; i++){
				// X_Coordinates
				int X_Coordinates;
					if(rand()%2 == 1){
						X_Coordinates = rand()%LHSRoad-widthBush+1;
					}else{
						X_Coordinates = (rand()%LHSRoad)+RHSRoad;
					}
							
				//Y_Coordinates
				int Y_Coordinates = (rand()%screen_height())-(screen_height()/2);
				
				Bush[i] = sprite_create(X_Coordinates,Y_Coordinates,widthBush,heightBush,Bush_image);
				if(Y_Coordinates>=screen_height()/5){
					sprite_draw(Bush[i]);
				}
			}
		}
		
	//initialize OldCars
		void initialize_OldCars(int OldCars_Count){
			
			int X_Coordinates;
			
			for(int i = 0; i<=OldCars_Count; i++){
				// X_Coordinates
				while((X_Coordinates<LHSRoad|| X_Coordinates>RHSRoad-widthOldCars )){
					X_Coordinates = rand()%(screen_width()-1);
				}
				//Y_Coordinates
				int Y_Coordinates = (-rand()%screen_height());
				
				OldCars[i] = sprite_create(X_Coordinates,Y_Coordinates,widthOldCars,heightOldCars,OldCars_image);
				if(Y_Coordinates>=screen_height()/5){
					sprite_draw(OldCars[i]);
				}
			}
		}
			
	//initialize Zombie
		void initialize_Zombies(int Zombie_Count){
				
			
			for(int i = 0; i<=Zombie_Count; i++){
				// X_Coordinates
				int X_Coordinates = rand()%screen_width();	
				//Y_Coordinates
				int Y_Coordinates = (rand()%screen_height())-(screen_height()/2);
				
				Zombie[i] = sprite_create(X_Coordinates,Y_Coordinates,widthZombie,heightZombie,Zombie_image);
				if(Y_Coordinates>=screen_height()/5){
					sprite_draw(Zombie[i]);
				}
			}
		}

	//initialize GasStation
		void initialize_GasStation(void){
			int X_Coordinates;
			
			if(rand()%2 == 1){
				X_Coordinates = RHSRoad+1;
			}else{
				X_Coordinates = LHSRoad-widthGasStation-1;
			}
			
			//Y_Coordinates
			int Y_Coordinates = (rand()%screen_height())-(screen_height()/2);
				
			GasStation = sprite_create(X_Coordinates,Y_Coordinates,widthGasStation,heightGasStation,GasStation_image);
			if(Y_Coordinates>=screen_height()/5){
				sprite_draw(GasStation);
			}
		}

	//Move and draw scenery
		void draw_Scenery(sprite_id scenery,int spawnHeight,int remainder, int Type){
			if(sprite_y(scenery)>screen_height()+remainder){
				// X_Coordinates
				int X_Coordinates;
				
				if(Type == 0){ //Bush or Tree
					if(rand()%2 == 1){
						X_Coordinates = rand()%LHSRoad-widthTree+1;
					}else{
						X_Coordinates = (rand()%LHSRoad)+RHSRoad;
					}
				}else if(Type == 1){ //Old Cars
					while((X_Coordinates<LHSRoad|| X_Coordinates>RHSRoad-widthOldCars )){
						X_Coordinates = rand()%(screen_width()-1);
					}
				}else if(Type == 2){ //Zombies
					X_Coordinates = rand()%screen_width();
				}else if(Type == 3){ //GasStation
					if(rand()%2 == 1){
						X_Coordinates = RHSRoad+1;
					}else{
						X_Coordinates = LHSRoad-widthGasStation-1;
					}
				}else{ //RoadLines
					X_Coordinates = screen_width()/2-1;
				}
								
				int Y_Coordinates;
				//Y_Coordinates
				if(Type != 4){
					Y_Coordinates = -rand()%spawnHeight;
				}else{
					Y_Coordinates = screen_height()/5;
				}
				sprite_move_to(scenery,X_Coordinates,Y_Coordinates);
			}else{
				sprite_move(scenery,0,CurrentSpeed/10);
			}
			if(sprite_y(scenery)>=screen_height()/5){
				sprite_draw(scenery);
			}	
		}
	
	// draw finish line at winning distance, also hides sprites after the finish line.
	void FinishLine(){
		//Two Lines to make the finish Line
		int FinishLineHeight1 = -Winning_Distance/10+6*screen_height()/9+screen_height()/5+1+DistanceTravelled/10;
		int FinishLineHeight2 = -Winning_Distance/10+5.5*screen_height()/9+screen_height()/5+1+DistanceTravelled/10;
		
		//Draw the first line
		if(FinishLineHeight1 >= screen_height()/5){
			draw_line(1,FinishLineHeight1,screen_width()-2,FinishLineHeight1,'_');
			for(int i = 1;i<FinishLineHeight1-FinishLineHeight2;i++){
				draw_line(1,FinishLineHeight1-i,screen_width()-2,FinishLineHeight1-i,' ');
			}
		}
		
		//Draw the second line and write FinishLine inbetween the two.
		if(FinishLineHeight2 >= screen_height()/5){
			draw_line(1,FinishLineHeight2,screen_width()-2,FinishLineHeight2,'_');
			draw_string(screen_width()/2-1,(FinishLineHeight1-FinishLineHeight2)/2+FinishLineHeight2+1,"Finish Line");
		}
		
		//For every line after the finish line draw a blank line, used to hide sprites.
		for(int i = 1; i<100; i++){
			draw_line(1,FinishLineHeight2-i,screen_width()-2,FinishLineHeight2-i,' ');
		}
	}
	
//Records how long car has been stationary at gas station.
void Refill(void){
	if(StopTimeGas == 0){
		StopTimeGas = get_current_time();
	}else{
		if(StopTimeGas - get_current_time() < 3){
			FuelRemaining += 0.33;
		}
		if(FuelRemaining > 100){
			FuelRemaining = 100;
		}
	}
}
		
//Refuel RaceCar	
//refuel lasts 3 seconds if tank is empty, else it will refuel at the same rate as if it was empty
	void Refuel(void){
		int RaceCarLeft = round(sprite_x(RaceCar));
		int RaceCarTop = round(sprite_y(RaceCar));
		int RaceCarRight = RaceCarLeft + widthRaceCar-1;
		int RaceCarBottom = RaceCarTop + heightRaceCar-1;
		
		int GasStationTop = round(sprite_y(GasStation));
		int GasStationBottom = GasStationTop + heightGasStation-1;
		int GasStationXLocation = sprite_x(GasStation);
		
		if(GasStationXLocation == RHSRoad+1){
			if(RaceCarTop <= GasStationBottom && RaceCarTop >= GasStationTop && CurrentSpeed == 0 && FuelRemaining < 100 && RaceCarLeft > screen_width()/2){
				Refill();
			}else if(RaceCarBottom <= GasStationBottom && RaceCarBottom >= GasStationTop && CurrentSpeed == 0 && FuelRemaining < 100 && RaceCarLeft > screen_width()/2){
				Refill();
			}
		}else{
			if(RaceCarTop <= GasStationBottom && RaceCarTop >= GasStationTop && CurrentSpeed == 0 && FuelRemaining < 100 && RaceCarRight < screen_width()/2){
				Refill();
			}else if(RaceCarBottom <= GasStationBottom && RaceCarBottom >= GasStationTop && CurrentSpeed == 0 && FuelRemaining < 100 && RaceCarRight < screen_width()/2){
				Refill();
			}
			if(CurrentSpeed != 0){
				StopTimeGas = 0;
			}
		}
	}

// Check no objects spawn ontop of each other
	void CheckNoSpawningCollisions(sprite_id AllSprites[], int posInArray){
	//Place left,right,top and bottoms coords for each sprite in an array.
	int LeftSprite[posInArray];
	int RightSprite[posInArray];
	int TopSprite[posInArray];
	int BottomSprite[posInArray];
		
	//Populate above arrays
	for(int i = 0; i<=posInArray;i++){	
		LeftSprite[i] = round(sprite_x(AllSprites[i]));
		RightSprite[i] = LeftSprite[i] + sprite_width(AllSprites[i]);
			
		TopSprite[i] = round(sprite_y(AllSprites[i]))+1;
		BottomSprite[i] = TopSprite[i] + sprite_height(AllSprites[i]);
	}
	//Check no overlap
	int counter = 1;
	for(int j = 0;j<posInArray;j++){
		for(int i = counter; i<=posInArray;i++){
			if(TopSprite[j]<BottomSprite[i] && TopSprite[i]<BottomSprite[j] && LeftSprite[i]<RightSprite[j] && LeftSprite[j]<RightSprite[i]){
				sprite_move_to(AllSprites[i],screen_width()/2,screen_height()+20);
			}
		}
		counter++;
	}
}

// Extenstion
	//initialize AI Cars
		void initialize_AICars(int AICar_Count){
			
			for(int i = 0; i<=AICar_Count; i++){
				// X_Coordinates
					int X_Coordinates = rand()%screen_width();	
					//Y_Coordinates
					int Y_Coordinates = -rand()%3*screen_height();
					
				AICars[i] = sprite_create(X_Coordinates,Y_Coordinates,widthAICar,heightAICar,AICar_image);
				if(Y_Coordinates>=screen_height()/5){
					sprite_draw(AICars[i]);
				}
			}
		}
		
	// AI Cars
		void draw_AICars(sprite_id AI_Car){	
			if(sprite_y(AI_Car)>= screen_height()/5){
				sprite_draw(AI_Car);
			}
		}
		
	// Randomly accelerate/decelerate and move AI cars
		void Move_AICars(sprite_id AICar, int Max_Speed_Set, int counter){	
			if(CrashAI[counter] == false){
				double speed;
				double horizontal;
				if(sprite_x(AICar) > RHSRoad || sprite_x(AICar)+widthAICar-1 < LHSRoad ){
					//Make car a bit faster than player off road to off set the random speed the ai selects
					Max_Speed_Set = 5;
				}
				
				//Random speed is selected by the AI car, but the following code ensures the acceleration can only be 1 or -1
				speed = -rand()%Max_Speed_Set;
				if((CurrentSpeedAI[counter]-speed) > 1){
					// minus 1 as a positive speed is negative as car is travelling up screen
					CurrentSpeedAI[counter]--;
				}else if((CurrentSpeedAI[counter]-speed) < -1){
					CurrentSpeedAI[counter]++;
				}
				
				//Randomly select which direction the car is going to move.
				switch(rand()%3){
					case 0:
						horizontal = 0.1;
						break;
					case 1:
						horizontal = -0.1;
						break;
					default:
						horizontal = 0;
						break;
				}
						
				//Make sure AI cars cannot move faster than 10 speed
				if(CurrentSpeedAI[counter] >10){
					CurrentSpeedAI[counter] = 10;
				}
				
				sprite_move(AICar,horizontal,CurrentSpeedAI[counter]/10+CurrentSpeed/10);
			}else{
				sprite_move(AICar,0,CurrentSpeed/10);
			}
			draw_AICars(AICar);
		}
		
	// Check Position in race
		int Position(int AICar_Count, bool GameEnd){
			int Y_Coordinate_Player = round(sprite_y(RaceCar));
			int Position = 1;
			
			//Find Players position
			for(int i = 0; i < AICar_Count;i++){
				if(round(sprite_y(AICars[i])) < Y_Coordinate_Player){
					Position++;
				}
			}
			
			//Display final position on end screen if the game is over.
			if(GameEnd == false){
				draw_int((11*screen_width()-4)/12-2,2*screen_height()/20+1,Position);
			}
			return Position;
		}
	
	//Moves the AICar 10 places away from the player car if a collision occurs.
	void Fix_AICar_Location(sprite_id AICar){
			if(sprite_x(RaceCar) > sprite_x(AICar)){
				// Ensure players car cannot leave the playing area
				if(sprite_x(RaceCar) <= screen_width()-7){
					sprite_move_to(AICar,round(sprite_x(AICar))-5,round(sprite_y(AICar)));
					sprite_move_to(RaceCar,round(sprite_x(RaceCar))+5,round(sprite_y(RaceCar)));
				}else{
					sprite_move_to(AICar,round(sprite_x(AICar))-10,round(sprite_y(AICar)));
				}
			}else{
				// Ensure players car cannot leave the playing area
				if(sprite_x(RaceCar) >= 7){
					sprite_move_to(AICar,round(sprite_x(AICar))+5,round(sprite_y(AICar)));
					sprite_move_to(RaceCar,round(sprite_x(RaceCar))-5,round(sprite_y(RaceCar)));
				}else{
					sprite_move_to(AICar,round(sprite_x(AICar))+10,round(sprite_y(AICar)));
				}
			}
	}
	
	//Check if player car collides with an AICar
		bool CollidedAICar(sprite_id AICar){
			int RaceCarLeft = round(sprite_x(RaceCar));
			int RaceCarTop = round(sprite_y(RaceCar));
			int RaceCarRight = RaceCarLeft + widthRaceCar-1;
			int RaceCarBottom = RaceCarTop + heightRaceCar-1;
			
			int AICarLeft = round(sprite_x(AICar));
			int AICarTop = round(sprite_y(AICar));
			int AICarRight = AICarLeft + sprite_width(AICar)-1;
			int AICarBottom = AICarTop + sprite_height(AICar)-1;
				
			if(AICarTop>RaceCarBottom) return false;
			if(RaceCarTop>AICarBottom) return false;
			if(RaceCarLeft>AICarRight) return false;
			if(AICarLeft>RaceCarRight) return false;
			
			Condition -= 10;
			if(Condition <= 0){
				return true;
			}else{
				Fix_AICar_Location(AICar);
				return false;
			}
		}

	//Check if AICar collides with an item of scenery
		bool AICollideScenery(sprite_id AICar,sprite_id scenery){
			int AICarLeft = round(sprite_x(AICar));
			int AICarTop = round(sprite_y(AICar));
			int AICarRight = AICarLeft + widthAICar-1;
			int AICarBottom = AICarTop + heightAICar-1;
			
			int SceneryLeft = round(sprite_x(scenery));
			int SceneryTop = round(sprite_y(scenery));
			int SceneryRight = SceneryLeft + sprite_width(scenery)-1;
			int SceneryBottom = SceneryTop + sprite_height(scenery)-1;
			
			if(AICarLeft == SceneryLeft){
				return false;
			}
				
			if(SceneryTop>AICarBottom) return false;
			if(AICarTop>SceneryBottom) return false;
			if(AICarLeft>SceneryRight) return false;
			if(SceneryLeft>AICarRight) return false;
			
			return true;			
		}
	
	//Compile Extensions
		int Extensions(int AICar_Count,int Max_Speed_Set,int posInArray,sprite_id AllSprites[], int Mode){
			for(int i = 0; i < AICar_Count;i++){
				Move_AICars(AICars[i],Max_Speed_Set,i);
			}
			if(Mode == 1){
				Position(AICar_Count,false);
			}
			
			//Check for collisions with AI Cars
			for(int i = 0; i < AICar_Count; i++){	
				if(CollidedAICar(AICars[i]) == true) return 1;
			}
			
			//Check for collisions between AI Cars and scenery
			for(int j = 0; j < AICar_Count; j++){	
				for(int i = 0; i < posInArray; i++){
					if(CrashAI[j] == false){
						CrashAI[j] = AICollideScenery(AICars[j],AllSprites[i]);
					}
				}
			}
			return 0;
		}
		
//Move RaceCar
	void MoveRaceCar(){
		// Set Max and Min speeds for players car
		int MaxSpeed;
		int MinSpeed = 0;
		
		//get key input		
		int key = get_char();

		// Horizontal Movement, Check if car is moving forward first before it can move laterally. 
			//	 Move race car left, ensuring that it always remains within the screen.Greater than 1 as there is a border.
			if(CurrentSpeed != 0){	
				if (key == 'a'){
					if(sprite_x(RaceCar)>1){
						sprite_move(RaceCar,-1,0);
					}
				}
				//	 Move race car right, ensuring that it always remains within the screen. Minus 1 as there is a border, uses less than as then it cannot go past screen width
				if (key == 'd'){
					if(sprite_x(RaceCar)+widthRaceCar < screen_width()-1){
						sprite_move(RaceCar,1,0);
					}
				}
				if((sprite_x(RaceCar)<LHSRoad || sprite_x(RaceCar)+widthRaceCar-1>RHSRoad) && CurrentSpeed >3){
					CurrentSpeed = 3;
				}
			}
		
		// Vertical speed
		
			// Check car is on the road, if it is not set the maxspeed to 3
			if(sprite_x(RaceCar)<LHSRoad || sprite_x(RaceCar)+widthRaceCar-1>RHSRoad){
				MaxSpeed = 3;
			}else{
				MaxSpeed = 10;
			}
			
			//Increase car speed, check racecar cannot go faster than max speed
			if (key == 'w' && CurrentSpeed<MaxSpeed){
				CurrentSpeed++;
			}

			//Decrease car speed, check racecar cannot get a negative speed.
			if (key == 's' && CurrentSpeed > MinSpeed){
				CurrentSpeed--;
			}
	}

//Draw Sprites
//Scenery can purposely spawn off screen so only the edge of it can be see.
	void Draw_Sprites(int RoadLines_Count,int Tree_Count,int Bush_Count,int OldCars_Count,int Zombie_Count, sprite_id GasStation){
		draw_Roads();
		
		//Finds remainder for road draw statement
		int remainder = screen_height()%4;
			if(remainder == 0){
				remainder = 2;
			}else if(remainder == 1){
				remainder = 1;
			}else if(remainder == 3){
				remainder = 11;				
			}else{
				remainder = 0;
			}
			
		//draw_RoadLines(RoadLines_Count);
		for(int i = 0; i<=RoadLines_Count; i++){
			draw_Scenery(RoadLines[i],0,remainder,4);
		}
		//Draw Trees
		for(int i = 0; i<=Tree_Count; i++){
			draw_Scenery(Tree[i],10,0,0);
		}
		//Draw Bushes
		for(int i = 0; i<=Bush_Count; i++){
			draw_Scenery(Bush[i],10,0,0);
		}
		//Draw OldCars
		for(int i = 0; i<=OldCars_Count; i++){
			draw_Scenery(OldCars[i],100,0,1);
		}
		//Draw Zombies
		for(int i = 0; i<=Zombie_Count; i++){
			draw_Scenery(Zombie[i],10,0,1);
		}
		//Draw Gas station
		draw_Scenery(GasStation,40,0,3);
		
		//Draw Finish Line above scenery but below the race car, so the race car is still visiable as it drives over the finish line.
		FinishLine();
		
		//	Draw the race car.
		sprite_draw(RaceCar);
		
		//draw bordeer, this is drawn last so it will draw over anything on edge
		Border(0,screen_width()-1,0,screen_height()-1,'*');	
	}

//Collisions	
	int Collisions(int RoadLines_Count,int Tree_Count,int Bush_Count,int OldCars_Count,int Zombie_Count, sprite_id GasStation,int posInArray, sprite_id AllSprites[]){
		//Checks if car collides with a tree
		for(int i=0; i<=Tree_Count;i++){
			GameOver = CollidedScenery(Tree[i]);
			if(GameOver == true) return 1;
		}
		
		//Checks if car collides with a Zombie
		for(int i=0; i<=Zombie_Count;i++){
			GameOver = CollidedScenery(Zombie[i]);
			if(GameOver == true) return 1;
		}
		
		//Checks if car collides with a Bush
		for(int i=0; i<=Bush_Count;i++){
			GameOver = CollidedScenery(Bush[i]);
			if(GameOver == true) return 1;
		}
		
		//Checks if car collides with an OldCar
		for(int i=0; i<=OldCars_Count;i++){
			GameOver = CollidedScenery(OldCars[i]);
			if(GameOver == true) return 1;
		}
		
		//Checks when car respawns it does not respawn ontop of a zombie
		for(int i=0; i<=Zombie_Count;i++){
			Check_Location(Zombie[i]);
		}
		
		//Checks when car respawns it does not respawn ontop of an OldCar
		for(int i=0; i<=OldCars_Count;i++){
			Check_Location(OldCars[i]);
		}
		
		//Checks if car collides with a gasstation
		GameOver = Check_Location(GasStation);
		if(GameOver == true) return 1;
		
		//Checks when a sprite spawns it does not spawn ontop of anything else
		CheckNoSpawningCollisions(AllSprites, posInArray);
		return 0;
	}
	
// Play game
	void process(int RoadLines_Count, int Tree_Count,int Bush_Count,int OldCars_Count, int Zombie_Count, int AICar_Count, int Max_Speed_Set, int Mode) {
		//Create array of all sprites
		sprite_id AllSprites[NumberOfSprites];
		int posInArray = 0;
		
		//Populate array with every sprite in the game except players car and AIcars
		for(int i = 0;i<=Tree_Count;i++){
			AllSprites[posInArray] = Tree[i];
			posInArray++;
		}
		for(int i = 0;i<=Zombie_Count;i++){
			AllSprites[posInArray] = Zombie[i];
			posInArray++;
		}
		for(int i = 0;i<=Bush_Count;i++){
			AllSprites[posInArray] = Bush[i];
			posInArray++;
		}
		for(int i = 0;i<=OldCars_Count;i++){
			AllSprites[posInArray] = OldCars[i];
			posInArray++;
		}
		//Gas Station	
		AllSprites[posInArray] = GasStation;
		
		//Move RaceCar
		MoveRaceCar();

		Draw_Sprites(RoadLines_Count,Tree_Count,Bush_Count,OldCars_Count,Zombie_Count,GasStation);
		Refuel();

		//Extenstions
		Extensions(AICar_Count,Max_Speed_Set,posInArray,AllSprites,Mode);	
		
		Collisions(RoadLines_Count,Tree_Count,Bush_Count,OldCars_Count,Zombie_Count,GasStation,posInArray,AllSprites);
		
		
	}
	
// Splash screen
	void Splash_Screen(){
		
		//Draw Borders
		Border(0,screen_width()-1,0,screen_height()-1,'*');
			
		//Name and Student number
		draw_string(screen_width()/2-12,2*screen_height()/10,"Race to Zombie Mountain");
		draw_string(screen_width()/2,8*screen_height()/10+1,"Liam Hulsman-Benson, N9960392");
				
		//Controls
		draw_string(screen_width()/2-20,8*screen_height()/10-1,"Controls");
		char text[4][20] = {"W = faster","S = Slower","A = Go left","D = Go right"};
		for(int i=0;i<=3;i++){
			draw_string(screen_width()/2-20,8*screen_height()/10+i,text[i]);
		}
		
		//Instructions
			// Make it work on different sized screens
		draw_string(screen_width()/2-6,3*screen_height()/10,"Instructions");
		draw_string(screen_width()/2-27,3*screen_height()/10+1,"Your aim is to make it to Zombie Mountain, you have to");
		draw_string(screen_width()/2-25,(3*screen_height()/10)+2,"avoid the obstacles on Your path. If you leave the");
		draw_string(screen_width()/2-25,(3*screen_height()/10)+3,"road your car is limited to the third speed.If your");
		draw_string(screen_width()/2-25,(3*screen_height()/10)+4,"car is running low on fuel, stop at a fuel station");
		draw_string(screen_width()/2-26,(3*screen_height()/10)+5,"for 3 seconds before leaving again. To refuel stop so");
		draw_string(screen_width()/2-26,(3*screen_height()/10)+6,"at least one unit of the car overlaps the gas station");
		draw_string(screen_width()/2-27,(3*screen_height()/10)+7,"verticaly. You also need to be over the dotted line and");
		draw_string(screen_width()/2-25,(3*screen_height()/10)+8,"be on the same side of the road as the gas station.");
		
		//Press to select gamemode
		draw_string(screen_width()/2-17,(3*screen_height()/10)+10,"Press any key to select gamemode...");
		show_screen();
		wait_char();
	}

// Select Screen
	int Select(void){
		clear_screen();
		//Draw Borders
		Border(0,screen_width()-1,0,screen_height()-1,'*');
		
		draw_string(screen_width()/2-21,screen_height()/3,"Choose between Race Mode and Survival mode.");	
		draw_string(screen_width()/2-23,screen_height()/3+1,"Press R for race or any other key for survival");
		
		draw_string(2*screen_width()/4-23,3*screen_height()/5,"In race mode more AI cars spawn,less obstacles");	
		draw_string(2*screen_width()/4-8,3*screen_height()/5+1,"and no refueling");
		
		draw_string(2*screen_width()/4-25,4*screen_height()/5,"In Survival mode less AI cars spawn,more obstacles");	
		draw_string(2*screen_width()/4-11,4*screen_height()/5+1,"and there is refueling");
		
		show_screen();
		
		// If R or r is pressed game will replay, if any other key is pressed game will end. any other key was used to prevent errors, of a player pressing the wrong key.
		int key = wait_char();
		if(key == 'r' || key == 'R'){
			return 1;
		}
		return 0;
	}

//Find the suffix of final position	
	void EndingPosition(int FinalPosition){
		if(FinalPosition%10 == 1){
			if(FinalPosition/10 == 1){
				draw_string(2*(screen_width()/4)+2,2*screen_height()/4+2,"th");
			}else{
				draw_string(2*(screen_width()/4)+2,2*screen_height()/4+2,"St");
			}
		}else if(FinalPosition%10 == 2){
			if(FinalPosition/10 == 1){
				draw_string(2*(screen_width()/4)+2,2*screen_height()/4+2,"th");
			}else{
				draw_string(2*(screen_width()/4)+2,2*screen_height()/4+2,"nd");
			}
		}else if(FinalPosition%10 == 3){
			if(FinalPosition/10 == 1){
				draw_string(2*(screen_width()/4)+2,2*screen_height()/4+2,"th");
			}else{
				draw_string(2*(screen_width()/4)+2,2*screen_height()/4+2,"rd");
			}
		}else{
			draw_string(2*(screen_width()/4)+2,2*screen_height()/4+2,"th");
		}
	}

//Checks if user selects to play a new game, after previous game has ended
bool NewGameCheck(void){
	int key = wait_char();
		if(key == 'Y' || key == 'y'){
			NewGame = true;
			GameOver = false;
			CurrentSpeed = 0;
			Condition = 100;
			DistanceTravelled = 0;
			FuelRemaining = 100;
		}else{
			NewGame = false;	
		}
	return NewGame;
}
	
//Displays the corresponding text for the game mode. if won.
void DisplayTextWon(int Mode,int AICar_Count, double finishTime){
	if(Mode == 1){
		int FinalPosition = Position(AICar_Count,true);
		EndingPosition(FinalPosition);
		draw_string(1*(screen_width()/2)-22,1*screen_height()/4,"Congratulations, You reached Zombie Mountain!");
		draw_string(2*screen_width()/4-6,2*screen_height()/4,"You finished:");
		draw_double(2*(screen_width()/4)-1,2*screen_height()/4+2,FinalPosition);
		
		//Ask if user wants to play new game.
		draw_string(screen_width()/2-15,3*screen_height()/4-1,"Would you like to play again?");
		draw_string(screen_width()/2-3,3*screen_height()/4,"Y or N");
	}else{
		draw_string(1*(screen_width()/2)-22,1*screen_height()/4,"Congratulations, You made it to Zombie Mountain");
		draw_string(1.5*screen_width()/4-14,3*screen_height()/4,"You got to Zombie Mountain in:");
		draw_double(1.5*(screen_width()/4)-1,3*screen_height()/4+2,finishTime);
		draw_string(1.5*(screen_width()/4)-4,3*screen_height()/4+4,"Seconds");
		draw_string(2.5*screen_width()/4-7,3*screen_height()/4,"You travelled:");
		draw_int(2.5*(screen_width()/4)-2,3*screen_height()/4+2,DistanceTravelled);
		draw_string(2.5*(screen_width()/4)-3,3*screen_height()/4+4,"Metres");
		//Ask if user wants to play new game.
		draw_string(screen_width()/2-15,screen_height()/2-1,"Would you like to play again?");
		draw_string(screen_width()/2-3,screen_height()/2,"Y or N");
	}
}

//Displays the corresponding text for the game mode. if lost.
void DisplayTextLost(int Mode,int AICar_Count){
	if(Mode != 1){
		draw_string(screen_width()/2-9,screen_height()/4,"You're Zombie Food!");
		draw_string(2*screen_width()/4-8,3*screen_height()/4,"You travelled:");
		draw_int(2*(screen_width()/4)-2,3*screen_height()/4+2,DistanceTravelled);
		draw_string(2*(screen_width()/4)-3,3*screen_height()/4+4,"Metres");
		//Ask if user wants to play new game.
		draw_string(screen_width()/2-15,screen_height()/2-1,"Would you like to play again?");
		draw_string(screen_width()/2-3,screen_height()/2,"Y or N");
	}else{
		int FinalPosition = Position(AICar_Count,true);
		EndingPosition(FinalPosition);
		draw_string(1*(screen_width()/2)-6,1*screen_height()/4,"You Crashed!");
		draw_string(2*screen_width()/4-6,2*screen_height()/4,"You finished:");
		draw_double(2*(screen_width()/4)-1,2*screen_height()/4+2,FinalPosition);
		//Ask if user wants to play new game.
		draw_string(screen_width()/2-15,3*screen_height()/4-1,"Would you like to play again?");
		draw_string(screen_width()/2-3,3*screen_height()/4,"Y or N");
	}
}

// Game Over screen
	bool GameOver_Screen(double startTime, int Mode, int AICar_Count){
		double finishTime = round((get_current_time()-startTime)*100)/100;
		
		//Draw Borders
		Border(0,screen_width()-1,0,screen_height()-1,'*');
		
		if(DistanceTravelled >= Winning_Distance){
			DisplayTextWon(Mode, AICar_Count,finishTime);	
		}else{
			DisplayTextLost(Mode, AICar_Count);		
		}
		
		show_screen();
		
		//Clear input buffer, Pause to prevent skipping end screen.
		timer_pause(1000);
		while ( get_char() >= 0 ) { /* loop until no chars buffered */ }
		
		return NewGameCheck();	
	}
	
//Check if Zombie Mountain has been reached
	void Zombie_Mountain(void){
		if(DistanceTravelled >= Winning_Distance){
			GameOver = true;
		}
	}
	
//Initialize_All
	void Initialize_All(int RoadLines_Count,int Tree_Count,int Bush_Count,int Zombie_Count,int OldCars_Count,int AICar_Count){
		initialize_RoadLines(RoadLines_Count);
		initialize_Trees(Tree_Count);	
		initialize_Bush(Bush_Count);
		initialize_Zombies(Zombie_Count);	
		initialize_OldCars(OldCars_Count);
		initialize_AICars(AICar_Count);
		initialize_GasStation();
	}
	
//Change values so they are appropriate for the game mode
int main(void) {		
	setup_screen();	
	Splash_Screen();
	
	//Seed for rand() based on current system time, so game will never be the same.			
	srand(get_current_time());	
	
	int Tree_Count;
	int Bush_Count;
	int Zombie_Count;
	int OldCars_Count;
	int AICar_Count;
	int Max_Speed_Set;
	double startTime = get_current_time();
	int RoadLines_Count = screen_height()/4;
	
	while(NewGame == true){
		int Mode = Select();
		if(Mode == 1){
			Tree_Count = screen_width()/50;
			Bush_Count = screen_width()/50;
			Zombie_Count = screen_width()/20;
			OldCars_Count = 10;
			AICar_Count = 40;
			Max_Speed_Set = 10;
		}else{
			Tree_Count = screen_width()/20;
			Bush_Count = screen_width()/20;
			Zombie_Count = screen_width()/10;
			OldCars_Count = screen_width()/50;
			AICar_Count = 20;
			Max_Speed_Set = 8;
		}
	
		Setup();
		show_screen();
		draw_Roads();
	
		Initialize_All(RoadLines_Count,Tree_Count,Bush_Count,Zombie_Count,OldCars_Count,AICar_Count);
		
		while(!GameOver){	
			process(RoadLines_Count,Tree_Count,Bush_Count,OldCars_Count,Zombie_Count,AICar_Count,Max_Speed_Set,Mode);
			timer_pause(10);
			DashBoard(startTime,Mode);
			show_screen();
			Zombie_Mountain();
			//Clears screen for new inputs to be displayed
			clear_screen();
		}
		NewGame = GameOver_Screen(startTime,Mode,AICar_Count);
	}
}