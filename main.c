#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Yollari karsilastirirken en ust deger olarak kullanilacak sayi
// Maximum number to use as the top value when comparing paths
#define INFINITY 9999999

// Graph icerisindeki uc nokta sayisini belirtiyoruz
// We specify the number of edge points in the graph
#define N 7

// Graph icin struct
// struct for Graph
struct Graph {
	// Bitişiklik listesini temsil etmek için Node işaretçiler dizisi
	// An array of pointers to Node to represent adjacency list
	struct Node* head[N];
};

// Graph'daki sehirleri icin yakinlik iliskisi temsili amacli Node struct'i
// Node struct for adjacency representation for cities in Graph
struct Node {
	int dest, yol;
	char city_name[20];
	char dest_name[20];
	struct Node* next;
};

// Node'lar arasi yollari temsili Edge struct'i
// Edge struct that represents the roads between Nodes
struct Edge {
	int src, dest, yol;
	char city_name1[20];
	char city_name2[20];
};

// Verilen yollardan(edges) yakinlik listesi olusturma fonsiyonu
// Proximity list creation function from given edges
struct Graph* createGraph(struct Edge edges[], int n)
{
	unsigned i;

	// Graph icin hafızada yer belirliyoruz
	// Allocating memory for the Graph
	struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));

	// Uc noktalari(sehirleri) temsil eden head'ler
	// 'Head's are representing edge points(cities)
	for (i = 0; i < N; i++){
		graph->head[i] = NULL;
	}

	// Yollari Graph'a ekliyoruz
	// Adding Roads to the Graph
	for (i = 0; i < n; i++)
	{
		// 'Yol'un baslangic, bitis noktasini ve uzunlugunu aliyoruz
		// We get the start, end point and length of the 'Road'
		int src = edges[i].src;
		int dest = edges[i].dest;
		int yol = edges[i].yol;
		char city_name1[20];
		char city_name2[20];
		
		// Graph sehir isim eklenmesi
		// Adding City name to Graph
		strcpy(city_name1, edges[i].city_name1);
		
		// Graph sehir isim eklenmesi
		// Adding City name to Graph
		strcpy(city_name2, edges[i].city_name2);
		
		// Sehirler arasindaki iliski icin Node olusturuyoruz
		// We create Node for the relationship between cities
		struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
		newNode->dest = dest;
		newNode->yol = yol;
		strcpy((newNode->city_name), city_name1);
		strcpy((newNode->dest_name), city_name2);
		
		newNode->next = graph->head[src];
		
		graph->head[src] = newNode;
	}

	return graph;
}

// Graph'daki sehirleri, gidebilecekleri sehirlere ve uzakliklarina gore yazdirma fonksiyonu
// Printing function of cities in Graph according to the cities they can go and their distances
void printGraph(struct Graph* graph)
{
	int i;
	for (i = 0; i < N; i++)
	{
		// Current sehiri ve gidebilecegi sehirleri sirayla mesafeleriyle beraber yazdiriyoruz
		// We print the current city and the cities it can travel sequentially with their distances
		struct Node* ptr = graph->head[i];
		while (ptr != NULL)
		{
			
			printf("%s -> %s (%d)\t", (ptr->city_name), (ptr->dest_name), ptr->yol);
			ptr = ptr->next;
		}

		printf("\n");
	}
}

// Gecis mumkun mu dogrulamasi
// Checking for travel restriction
int isOkaytoTravel(struct Graph* graph, int from_node, int to_node){
	
	struct Node* from_ptr = graph->head[from_node];
	/*
	*	// 'from_node'un gidebilecegi sehirler taranarak
	*	'to_node' olup olmadigina gore 1 ya da 0 donduruyoruz
	*
	*	// By scanning the cities' from_node can go to
	*	We return 1 or 0 depending on whether there is 'to_node'
	*/
	while (from_ptr != NULL)
	{
		if(from_ptr->dest == to_node){
			return 1;
		}
		from_ptr = from_ptr->next;
	}
	return 0;
}

// Dijkstra methodu ile en kısa yolu buluyoruz
// We find the shortest path with the Dijkstra method.
void findPath(struct Graph* graph, int G[N][N], int n, int startnode, int endnode)
{
 
	int cost[N][N],distance[N],pred[N];
	int visited[N],count,mindistance,nextnode,i,j;
	int path[N];
	
	/*
	* 	// 'pred[]' bir node'a gelene kadar gecilen node'lari sakliyor
	* 	'count' o ana kadar gecilen node'lari sayiyor
	* 	'cost' matriksi, gecis izinleri ile birlikte sehirler arasi mesafeyi temsil ediyor
	*	
	*	// 'pred[]' saves the passed nodes before arriving another node
	*	'count' counts the currently passed nodes
	*	'cost' matrix represents the distance between cities, together with permits
	*/
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(G[i][j]==0){
				cost[i][j]=INFINITY;
			}
			else{
				cost[i][j]=G[i][j];
			}
			
		}
	}
	
	// pred[], distance[] and visited[] baslangic degerleri
	// pred[], distance[] and visited[] init values
	for(i=0;i<n;i++)
	{
		distance[i]=cost[startnode][i];
		pred[i]=startnode;
		visited[i]=0;
	}
	
	distance[startnode]=0;
	visited[startnode]=1;
	count=1;
	
	while(count<n-1)
	{
		mindistance=INFINITY;
		
		/*
		*	// nextnode en kisa mesafedeki node'u gosterir
		*	eger iki sehir arasinda baska bir sehir yoksa ve geciste sikinti yoksa
		*	direkt olarak aradaki mesafeyi alir
		*	
		*	// nextnode shows the node at the shortest distance
		*	if two cities doesn't have any cities in between and there is no restriction
		*	it takes directly the distance
		*/
		for(i=0;i<n;i++){
		
			if(distance[i] < mindistance && !visited[i])
			{
				mindistance=distance[i];
				nextnode=i;
			}
			
		}
		
		// nextnode icin daha iyi bir yol olup olmadigina bakiyoruz	
		// checking if there is a better way for nextnode		
		visited[nextnode]=1;
		for(i=0;i<n;i++){
		
			if(!visited[i]){
				
				if(mindistance + cost[nextnode][i] < distance[i]){
					
					distance[i]=mindistance+cost[nextnode][i];
					pred[i]=nextnode;
				}
			}
		}
		count++;
	}
	
	// Son islemler sonrasinda ana guzergah aktarimi yapiyoruz
	// Sending the main route after final operations
	int sayac = 0;
	path[sayac] = endnode;
	j=endnode;
	do
	{
		sayac++;
		j=pred[j];
		path[sayac] = j;
	}while(j!=startnode);
	
	printf("\n");

	if(distance[endnode] == INFINITY){
		printf("Couldn't find a valid way...\n");
	}
	else{
		int total_cities = sizeof(path)/sizeof(path[0]);
		printf("A valid route was found, route: ");
		for(int i=0; i<sayac+1; i++){
			printf("%s, ", graph->head[path[sayac - i]]->city_name);
		}
		printf("\n");
		printf("for %s total distance is: %d km", graph->head[endnode]->city_name, distance[endnode]);
	}
	
}

// main
int main(void)
{
	
	printf("Finding Shortest Way Between Two Cities");
	printf("\n---------------------------------------\n\n");
	printf("Cities of the graph:\n");
	printf("\t0 - Prague\n");
	printf("\t1 - Helsinki\n");
	printf("\t2 - Beijing\n");
	printf("\t3 - Tokyo\n");
	printf("\t4 - Jakarta\n");
	printf("\t5 - London\n");
	printf("\t6 - New York\n");
	printf("\n\n");
	
	/* 	City - Destination - Road
	*	__________________________
	*	Prague - Helsinki - 1845	+
	*	Prague - London - 1264		+
	*	Helsinki - Tokyo - 7815		+
	*	Beijing - Tokyo - 1303		+
	*	Beijing - London - 8132		+
	*	Beijing - New York - 11550	+
	*	Jakarta - Beijing - 4616	+
	*	Tokyo - Jakarta - 5782		+
	*	Tokyo - New York - 10838	+
	*	New York - London - 5567	+
	*	London - Tokyo - 9566		+
	*/
	
	/* 	City Numbers
	*  	____________
	*	0 - Prague
	*	1 - Helsinki
	*	2 - Beijing
	*	3 - Tokyo
	*	4 - Jakarta
	*	5 - London
	*	6 - New York
	*/
	 
	struct Edge edges[] =
	{
		{ 0, 1, 1845, "Prague", "Helsinki"}, { 0, 5, 1264, "Prague", "London"}, { 1, 3, 7815, "Helsinki", "Tokyo"}, { 2, 3, 1303, "Beijing", "Tokyo"},
		{ 2, 5, 8132, "Beijing", "London"}, { 2, 6, 11550, "Beijing", "New York"}, { 4, 2, 4616, "Jakarta", "Beijing"}, { 3, 4, 5782, "Tokyo", "Jakarta"},
		{ 3, 6, 10838, "Tokyo", "New York"}, { 6, 5, 5567, "New York", "London"}, { 5, 3, 9566, "London", "Tokyo"}
	};
	
	int G[N][N];
	for(int k=0; k<N; k++){
		for(int y=0; y<N; y++){
			G[k][y] = 0;
		}
	}
	
	// Yol (edge) sayisi
	// Road (edge) number
	int n = sizeof(edges)/sizeof(edges[0]);
	
	// Verilen yollardan Graph olusumu
	// Creating Graph from given Roads
	struct Graph *graph = createGraph(edges, n);
	
	for(int i=0; i<n; i++){
		if(isOkaytoTravel(graph, edges[i].src, edges[i].dest)){
			G[edges[i].src][edges[i].dest] = edges[i].yol;
		}
		if(isOkaytoTravel(graph, edges[i].dest, edges[i].src)){
			G[edges[i].dest][edges[i].src] = edges[i].yol;
		}
	}
	
	// Kullanici sehir alimi
	// User city input
	int sehir1 = -1;
	int sehir2 = -1;
	int durum = 1;
	char tekrar_tercihi;
	while(durum != 4){
		
		switch(durum){
			
			case 0:
				printf("\nWould you like to do run it again? For YES (y), for NO (n): ");
				scanf("%s", &tekrar_tercihi);
				if(tekrar_tercihi == 'y'){
					durum = 1;
					break;
				}
				else if(tekrar_tercihi == 'n'){
					durum = 4;
					break;
				}
				else{
					printf("Invalid preference entered. Try again.\n");
					durum = 0;
					break;
				}
			case 1:
				printf(" 1 - Enter the number of the first city: ");
				scanf("%d", &sehir1);
				if(sehir1 < 7 && sehir1 > -1){
					durum = 2;
					break;
				}
				else{
					printf("An invalid city number has been entered. Try again.\n");
					durum = 1;
					break;
				}
			case 2:
				printf(" 2 - Enter the number of the destination city: ");
				scanf("%d", &sehir2);
				if(sehir2 < 7 && sehir2 > -1){
					durum = 3;
					break;
				}
				else{
					printf("An invalid city number has been entered. Try again.\n");
					durum = 2;
					break;
				}
			case 3:
				findPath(graph, G, N, sehir1, sehir2);
				printf("\n");
				durum = 0;
				break;
			case 4:
				getchar();
				return 0;
				break;
			default:
				printf("something is wrong...\n");
		}
	}
	
	getchar();
	return 0;
}
