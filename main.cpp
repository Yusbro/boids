#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <iostream>

namespace Boid{
	struct Flock{
		Vector2 position;
		Vector2 direction;
	};
	
	void init(std::vector<Flock> &flocks){
		for(int i=0;i<100;i++){
			Flock temp_flock = {
				(Vector2){(float)GetRandomValue(0, 800), (float)GetRandomValue(0, 600)},
				(Vector2){0.0, 5.0}
			};
			flocks.push_back(temp_flock);
		}
	}
	
	void draw(std::vector<Flock> &flocks){
		for(Flock f : flocks){
			DrawCircle(f.position.x, f.position.y, 5, RED);	
		}	
	}

	Vector2 seperation(std::vector<Flock> &flocks, Flock origin){
		Vector2 avg_seperation;	
		float seperation_strength = 50;
		
		int counter = 0;
		for(Flock f : flocks){
			float distance = Vector2Distance(origin.position, f.position);
			if(distance < seperation_strength){
				avg_seperation.x += f.position.x - origin.position.x;
				avg_seperation.y += f.position.y - origin.position.y;
				counter++;
			}
		}
		avg_seperation.x /= -counter;
		avg_seperation.y /= -counter;

		return avg_seperation;
	}
	

	


	void update(std::vector<Flock> &flocks){
		for(Flock f : flocks){
			Vector2 mouse_pos = GetMousePosition();
			float distance = Vector2Distance(f.position, mouse_pos);
			if(distance > 10) continue;
			
			Vector2 seperation_val = seperation(flocks, f);
			
			//the actual stuff!!
			DrawCircle(f.position.x, f.position.y, 50, BLACK);
			DrawCircle(seperation_val.x + f.position.x, seperation_val.y + f.position.y, 10, GREEN);
		}
	}
}



int main(){
	InitWindow(800, 600, "STUFFIE");
	SetTargetFPS(60);
	
	std::vector<Boid::Flock> flocks;
	Boid::init(flocks);	

	while(!WindowShouldClose()){
	
		BeginDrawing();
			ClearBackground(RAYWHITE);
			
			Boid::update(flocks);
			Boid::draw(flocks);	
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
