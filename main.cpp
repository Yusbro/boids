#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <iostream>

namespace Boid{
	struct Flock{
		Vector2 position;
		Vector2 direction;
		int id;
	};
	
	void init(std::vector<Flock> &flocks){
		for(int i=0;i<100;i++){
			Flock temp_flock = {
				(Vector2){(float)GetRandomValue(0, 800), (float)GetRandomValue(0, 600)},
				(Vector2){0.0, 5.0},
				i
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
		Vector2 avg_seperation = (Vector2){0,0};	
		float seperation_strength = 50;
		
		int counter = 0;
		for(Flock f : flocks){
			float distance = Vector2Distance(origin.position, f.position);
			if(origin.id==f.id) continue;
			if(distance < seperation_strength){
				avg_seperation.x += f.position.x - origin.position.x;
				avg_seperation.y += f.position.y - origin.position.y;
				counter++;
			}
		}
		if(counter == 0) return (Vector2){0,0};


		avg_seperation.x /= -counter;
		avg_seperation.y /= -counter;
		
		return avg_seperation;
	}
	
	
	Vector2 cohesion(std::vector<Flock> &flocks, Flock origin){
		Vector2 avg_cohesion = (Vector2){0,0};
		float seperation_strength = 30;
		
		int counter = 0;
		for(Flock f : flocks){
			float distance = Vector2Distance(origin.position, f.position);
			if(origin.id==f.id) continue;
			if(distance > seperation_strength && distance < 50){
				avg_cohesion.x += f.position.x - origin.position.x;
				avg_cohesion.y += f.position.y - origin.position.y;
				counter++;
			}
		}

		
		if(counter == 0) return (Vector2){0,0};


		avg_cohesion.x /= counter;
		avg_cohesion.y /= counter;

		return avg_cohesion;
	}
	
	
	Vector2 alignment(std::vector<Flock> &flocks, Flock origin){
		Vector2 avg_alignment = (Vector2){0,0};
		float align_strength = 50;

		int counter = 0;
		for(Flock f : flocks){
			float distance = Vector2Distance(origin.position, f.position);
			if(origin.id == f.id) continue;
			
			if(distance < align_strength){
				avg_alignment.x += f.direction.x;
				avg_alignment.y += f.direction.y;
				counter++;
			}	
		}
		
		
		if(counter == 0) return (Vector2){0,0};

		avg_alignment = Vector2Normalize(avg_alignment);

		return avg_alignment;
	}


	void update(std::vector<Flock> &flocks){
		for(Flock f : flocks){
			Vector2 mouse_pos = GetMousePosition();
			float distance = Vector2Distance(f.position, mouse_pos);
			if(distance > 10) continue;
			
			Vector2 seperation_val = seperation(flocks, f);
			Vector2 cohesion_val = cohesion(flocks, f);
			Vector2 alignment_val = alignment(flocks, f);


			Vector2 avg_direction = (Vector2){0,0};// Vector2Add(seperation_val, cohesion_val);
			
			
			avg_direction = Vector2Add(avg_direction, seperation_val);
			avg_direction = Vector2Add(avg_direction, cohesion_val);
			//avg_direction = Vector2Add(avg_direction, alignment_val);
			
			//avg_direction.x/=2;
			//avg_direction.y/=2;


			//the actual stuff!!
			DrawCircle(f.position.x, f.position.y, 50, BLACK);
			DrawCircle(seperation_val.x + f.position.x, seperation_val.y + f.position.y, 10, GREEN);//green is seperation!!
			DrawCircle(cohesion_val.x + f.position.x, cohesion_val.y + f.position.y, 10, BLUE); //blue is cohesion!!
			DrawCircle(alignment_val.x + f.position.y, alignment_val.y + f.position.y, 10, YELLOW);
			
			DrawCircle(f.position.x + avg_direction.x, f.position.y + avg_direction.y, 12, VIOLET);
			DrawLine(f.position.x, f.position.y, avg_direction.x + f.position.x, avg_direction.x + f.position.y, VIOLET);
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
