#include <iostream>
#include <algorithm>
#include <allegro5/allegro.h>
#include <chrono>
#include <random>

const float FPS = 30;
int interactions;
const int NUM = 500;
int WIDTH = 1600;
int HEIGHT = 700;
float rectWidth = (WIDTH - 100)/NUM;

bool redraw;

ALLEGRO_BITMAP* data[NUM];
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;

int heights[NUM];
int n = sizeof(heights) / sizeof(heights[0]);

bool doexit;

void Shuffle(int arr[], int n);
void LoadArray();
void Swap(int& a, int& b);
void SwapRect(int a, int b);
void CreateRectangles();
void Update();

void BubbleSort(int n);

int Partition(int low, int high);
void QuickSort(int low, int high);


int main()
{
	if (rectWidth < 1)
	{
		std::cout << "Rects are too small!";
		return -1;
	}

	if (!al_init())
	{
		std::cout << "Failed to initialize Allegro!" << std::endl;
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer)
	{
		std::cout << "Failed to create timer!";
		return -1;
	}

	if (!al_install_keyboard()) {
		std::cout << "Failed to initialize input!";
		al_destroy_timer(timer);
		return -1;
	}

	display = al_create_display(WIDTH, HEIGHT);
	if (!display)
	{
		std::cout << "Error opening the window!";
		al_destroy_timer(timer);
		return -1;
	}

	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		std::cout << "Failed to create event queue!";
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	LoadArray();
	CreateRectangles();

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	
	al_clear_to_color(al_map_rgb(0, 0, 0));
	Update();
	al_flip_display();

	al_start_timer(timer);

	while (!doexit)
	{
		ALLEGRO_EVENT ev;

		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER) 
		{
			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
		{
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_S:	
			{
				auto start = std::chrono::system_clock::now();
				QuickSort(0, n - 1);
				auto end = std::chrono::system_clock::now();
				std::chrono::duration<double> elapsed_seconds = end - start;
				std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
				std::cout << "Interactions: " << interactions << "\n";
				interactions = 0;
			}
				break;
			case ALLEGRO_KEY_A:
				{
					Shuffle(heights, n);
					for (int i = 0; i < NUM; i++) al_destroy_bitmap(data[i]);
					CreateRectangles();
					al_clear_to_color(al_map_rgb(0, 0, 0));
					Update();
					al_flip_display(); 
				}
				break;
			case ALLEGRO_KEY_W:
				{
					auto start = std::chrono::system_clock::now();
					int n = sizeof(heights) / sizeof(heights[0]);
					BubbleSort(n);
					auto end = std::chrono::system_clock::now();
					std::chrono::duration<double> elapsed_seconds = end - start;
					std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
					std::cout << "Interactions: " << interactions << "\n";
					interactions = 0;
				}
				break;
			case ALLEGRO_KEY_D:
				
				break;
			}
		}
		
		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;
		}
		
	}

	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	for (int i = 0; i < NUM; i++) al_destroy_bitmap(data[i]);
	al_destroy_display(display);
	return 0;
}

void Shuffle(int arr[], int n)
{
	// To obtain a time-based seed 
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	// Shuffling our array 
	std::shuffle(arr, arr + n, std::default_random_engine(seed));
}

void LoadArray()
{
	for (int i = 0; i < NUM; i++)
	{
		heights[i] = i;
	}

	Shuffle(heights, n);
}

void Swap(int& a, int& b)
{
	int temp = b;
	b = a;
	a = temp;
}

void SwapRect(int a, int b)
{
	ALLEGRO_BITMAP* temp = data[a];
	data[a] = data[b];
	data[b] = temp;

	int temp_int = heights[a];
	heights[a] = heights[b];
	heights[b] = temp_int;

	al_clear_to_color(al_map_rgb(0, 0, 0));
	Update();
	al_flip_display();

	interactions++;
}

void CreateRectangles()
{
	for (unsigned int i = 0; i < NUM; i++)
	{
		data[i] = al_create_bitmap(rectWidth, heights[i]);
		al_set_target_bitmap(data[i]);
		al_clear_to_color(al_map_rgb(255, 255, 255));
		al_set_target_bitmap(al_get_backbuffer(display));
	}
}

void Update() 
{
	for (unsigned int i = 0; i < NUM; i++)
	{
		al_draw_bitmap(data[i], 50.0f + i*rectWidth, HEIGHT - 50.0f - heights[i], 0);
	}
}

void BubbleSort(int n)
{
	int i, j;
	bool swapped;
	for (i = 0; i < n - 1; i++)
	{
		swapped = false;
		for (j = 0; j < n - i - 1; j++)
		{
			if (heights[j] > heights[j + 1])
			{
				SwapRect(j, j+1);
				swapped = true;
			}
		}

		// IF no two elements were swapped by inner loop, then break 
		if (swapped == false)
			break;
	}
}

int Partition(int low, int high)
{
	int pivot = heights[high];    // pivot 
	int i = (low - 1);  // Index of smaller element 

	for (int j = low; j <= high - 1; j++)
	{
		// If current element is smaller than or 
		// equal to pivot 
		if (heights[j] <= pivot)
		{
			i++;    // increment index of smaller element 
			SwapRect(i, j);
		}
	}
	//Swap(heights[i + 1], heights[high]);
	SwapRect(i + 1, high);
	return (i + 1);
}

void QuickSort(int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
		   at right place */
		int pi = Partition(low, high);

		// Separately sort elements before 
		// partition and after partition 
		QuickSort( low, pi - 1);
		QuickSort( pi + 1, high);
	}
}

