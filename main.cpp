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
				(Vector2){0.0, 1.0},
				i
			};
			flocks.push_back(temp_flock);
		}
	}
	
	void draw(std::vector<Flock> &flocks){
		for(Flock f : flocks){
			DrawCircle(f.position.x, f.position.y, 5, RED);
			DrawLine(f.position.x, f.position.y, f.position.x + f.direction.x*15, f.position.y + f.direction.y*15, RED);
		}	
	}

	Vector2 seperation(std::vector<Flock> &flocks, Flock origin){
		Vector2 avg_seperation = (Vector2){0,0};	
		float seperation_strength = 20;
		
		int counter = 0;
		for(Flock f : flocks){
			float distance = Vector2Distance(origin.position, f.position);
			if(origin.id==f.id) continue;
			if(distance < seperation_strength){
				avg_seperation.x += (f.position.x - origin.position.x) * (seperation_strength - distance);
				avg_seperation.y += (f.position.y - origin.position.y) * (seperation_strength - distance);
				counter++;
			}
		}
		if(counter == 0) return (Vector2){0,0};


		avg_seperation.x /= -counter;
		avg_seperation.y /= -counter;
		
		//avg_seperation = Vector2Multiply(avg_seperation, (Vector2){2,2});
		return avg_seperation;
	}
	
	
	Vector2 cohesion(std::vector<Flock> &flocks, Flock origin){
		Vector2 avg_cohesion = (Vector2){0,0};
		float seperation_strength = 30;
		
		int counter = 0;
		for(Flock f : flocks){
			float distance = Vector2Distance(origin.position, f.position);
			if(origin.id==f.id) continue;
			if(distance > seperation_strength && distance < 5){
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

		//Vector2 mouse_pos = GetMousePosition();

		Vector2 avg_alignment = (Vector2){0,0};//Vector2Subtract(mouse_pos, origin.position);
		//avg_alignment = Vector2Divide(avg_alignment, (Vector2){2,2});


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
		for(Flock &f : flocks){
			//Vector2 mouse_pos = GetMousePosition();
			//float distance = Vector2Distance(f.position, mouse_pos);
			//if(distance > 10) continue;
			
			Vector2 seperation_val = seperation(flocks, f);
			Vector2 cohesion_val = cohesion(flocks, f);
			Vector2 alignment_val = alignment(flocks, f);


			Vector2 avg_direction = (Vector2){0,0};// Vector2Add(seperation_val, cohesion_val);
			
			
			avg_direction = Vector2Add(avg_direction, seperation_val);
			avg_direction = Vector2Add(avg_direction, cohesion_val);
			avg_direction = Vector2Add(avg_direction, alignment_val);
			
			avg_direction = Vector2Normalize(avg_direction);
			
			f.direction = avg_direction;
			
			f.position = Vector2Add(f.direction, f.position);
			

			if(f.position.x < 0) f.position.x = 799;
			if(f.position.x > 800) f.position.x = 1;

			if(f.position.y < 0) f.position.y = 599;
			if(f.position.y > 600) f.position.y = 1;
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
