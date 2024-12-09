#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CITY_NAME_LENGTH 50
#define MAX_INPUT_LENGTH 10
#define CITY_LIST_SIZE 10
#define FILE_NAME "climate_change_report.txt"
#define USERNAME "admin"
#define PASSWORD "password123"
// Structure to hold city climate data
struct SeasonalData {
    float winter;
    float spring;
    float summer;
    float autumn;
};

struct CityClimateData {
    char city_name[MAX_CITY_NAME_LENGTH]; 
    struct SeasonalData temperatures;    
    struct SeasonalData precipitation;   
};

void timer();
// Function declarations
void print_header();

void print_footer();

char* city_input();

float numeric_input(const char* message);

int droughtrisk(float, float, char*);

int floodrisk(float, float, char*);

int heatwaverisk(float, char*);

int airquality(float);

void display(int, int, int, int);

void DisplayGeographicalLocation(char city[50]);

void DisplayGeographicalEffect(char city[50]);

void save_to_file(char* city, float cutemp, float cupret, float AQI, float co2, 
float avgtemp, float avgpret, int drought, int flood, int heatwave, int air);

int is_valid_city(const char* city);

void print_city_details(struct CityClimateData city_data);

// Predefined list of valid cities with seasonal climate data
struct CityClimateData city_data[CITY_LIST_SIZE] = {

    {"Lahore", 18.0, 22.5, 30.0, 23.0, 50.0, 60.0, 65.0, 50.0},
    {"Karachi", 20.0, 24.0, 32.0, 26.0, 40.0, 50.0, 55.0, 45.0},
    {"Islamabad", 10.0, 20.0, 30.0, 24.0, 60.0, 70.0, 75.0, 65.0},
    {"Rawalpindi", 10.0, 20.0, 30.0, 24.0, 60.0, 70.0, 75.0, 65.0},
    {"Multan", 16.0, 22.0, 35.0, 26.0, 20.0, 30.0, 35.0, 25.0},
    {"Faisalabad", 16.0, 22.0, 35.0, 26.0, 20.0, 30.0, 35.0, 25.0},
    {"Gilgit", 2.0, 10.0, 18.0, 15.0, 50.0, 40.0, 30.0, 40.0},
    {"Quetta", 12.0, 18.0, 30.0, 22.0, 40.0, 35.0, 45.0, 40.0},
    {"Peshawar", 10.0, 20.0, 30.0, 25.0, 50.0, 60.0, 65.0, 55.0},
    {"Hyderabad", 25.0, 28.0, 35.0, 30.0, 50.0, 60.0, 65.0, 55.0}

};

// Predefined list of valid cities
const char* valid_cities[CITY_LIST_SIZE] = {"Lahore", "Karachi", "Islamabad", "Rawalpindi", "Multan", 
"Faisalabad", "Gilgit", "Quetta", "Peshawar", "Hyderabad"};

// Check if the entered city is valid
int is_valid_city(const char* city) {

    int is_valid = 0;

    for (int i = 0; i < CITY_LIST_SIZE; i++) {

        if (strcasecmp(city, valid_cities[i]) == 0) {

            is_valid = 1; // City is valid
            break;
        }
    }
    return is_valid;
}
int login() {
    char username[20];
    char password[20];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
	sleep(2);
    if (strcmp(username, USERNAME) == 0 && strcmp(password, PASSWORD) == 0) {
    	 sleep(2);
        system("cls");
        return 1; // Login successful
    } else {
        printf("Invalid\n");
        sleep(2);
        system("cls");
        return 0;
    }
   
    

}

int main() {
    print_header(); // Print the header of the project
    sleep(5);
    system("cls");
    do{
	}while(!login());
	system("cls");
	getchar();
    printf("\n\n\n\n");
    printf("Welcome to the Climate Change Impact Simulator\n");
    printf("This simulator models the effects of climate change in different cities of Pakistan.\n");
	printf("\n");
    while (1) {
		
        char* city_name = city_input();
		if(!is_valid_city(city_name)){
			printf("Invalid city Input.Enter city of Pakistan.\n");
			continue;
		}
        if (city_name == NULL) {
            printf("\nThanks for using the Climate Change Simulator. Goodbye!\n");
            break;
        }

        // Input Section
        printf("\n--- Input Section ---\n");

        float cutemp = numeric_input("Input Current temperature in degree Celsius: ");
        float cupret = numeric_input("Input Current Precipitation in percentage: ");
        
        // Input for AQI and CO2 emissions with expanded validation logic
        float AQI;
        int is_AQI_valid = 0;

        while (!is_AQI_valid) {

            AQI = numeric_input("Input Current AQI (Air Quality Index): ");

            if (AQI >= 0 && AQI <= 500) {

                is_AQI_valid = 1; // AQI is valid

            } else {

                printf("Invalid AQI. Please enter a value between 0 and 500.\n");
            }
        }

        float co2;

        int is_co2_valid = 0;

        while (!is_co2_valid) {

            co2 = numeric_input("Input CO2 emission rate (tons per year): ");

            if (co2 >= 0) {

                is_co2_valid = 1; // CO2 emission is valid

            } else {

                printf("Invalid CO2 emission rate. Please enter a positive value.\n");
            }
        }

        // Get the city's climate data
        struct CityClimateData city_climate;
        for (int i = 0; i < CITY_LIST_SIZE; i++) {

            if (strcasecmp(city_name, valid_cities[i]) == 0) {

                city_climate = city_data[i];
                break;
            }
        }

        // Calculate deviations for seasonal data
        float avg_temp = (city_climate.temperatures.winter + city_climate.temperatures.spring + city_climate.temperatures.summer + city_climate.temperatures.autumn) / 4;

    	float avg_precip = (city_climate.precipitation.winter + city_climate.precipitation.spring + city_climate.precipitation.summer + city_climate.precipitation.autumn) / 4;
        float tempdevi = cutemp - avg_temp;
        float pretdevi = avg_precip - cupret;
        float pretdiff = cupret - avg_precip;

        // Risk Calculation with location-specific adjustments
        int drought = droughtrisk(pretdevi, co2, city_name);
        int flood = floodrisk(pretdiff, co2, city_name);
        int heatwave = heatwaverisk(tempdevi, city_name);
        int air = airquality(AQI);
        system("cls");
		timer();
        // Display Climate Impact Predictions
        display(drought, flood, heatwave, air);

        // Geographical Information Section
        printf("\nWould you like to view the impact of geographical location on natural disasters in the city? (yes/no): ");

        char geo_choice[MAX_INPUT_LENGTH];

        fgets(geo_choice, sizeof(geo_choice), stdin);
        geo_choice[strcspn(geo_choice, "\n")] = '\0'; // Remove trailing newline

        if (strcasecmp(geo_choice, "yes") == 0) {
            DisplayGeographicalLocation(city_name);
		}
            // Geographical Effect on Global Warming Section
            printf("\nWould you like to know the effect of geographical location on global warming? (yes/no): ");

            char warming_choice[MAX_INPUT_LENGTH];
            fgets(warming_choice, sizeof(warming_choice), stdin);
            warming_choice[strcspn(warming_choice, "\n")] = '\0'; // Remove trailing newline

            if (strcasecmp(warming_choice, "yes") == 0) {

                DisplayGeographicalEffect(city_name);
            
        }

        // Save the results to a file
        save_to_file(city_name, cutemp, cupret, AQI, co2, avg_temp, avg_precip, drought, flood, heatwave, air);

        // Ask User if they want to run another simulation
        printf("\nDo you want to run another simulation? (yes/no): ");

        char user_choice[MAX_INPUT_LENGTH];

        fgets(user_choice, sizeof(user_choice), stdin);
        user_choice[strcspn(user_choice, "\n")] = '\0'; // Remove trailing newline

        if (strcasecmp(user_choice, "no") == 0) {

            printf("\nExiting the Climate Change Simulator. Goodbye!\n");
            break;
        }
    }

    print_footer(); // Print the footer of the project
    sleep(20);
    return 0;
}

// Function to print the project header
void print_header() {

    printf("\n\n\n\n\n\n\n\n\n========================================================================================================================\n\n");
    printf("\t\t\t\t\t\t\033[1mClimate Change Impact Simulator\033[0m\n\n");
    printf("========================================================================================================================\n");
    

}

// Function to print the project footer
void print_footer() {

    printf("\nThank you for using the Climate Change Impact Simulator!\n");
    printf("Please feel free to come back and try with different cities or scenarios.\n");
    printf("========================================\n");

}

// Function to get valid city name input
char* city_input() {

    char* city = (char*)malloc(MAX_CITY_NAME_LENGTH * sizeof(char));
    printf("Enter the name of the city (or type 'exit' to quit): ");
    fgets(city, MAX_CITY_NAME_LENGTH, stdin);
    city[strcspn(city, "\n")] = '\0'; // Remove newline character from input

    if (strcasecmp(city, "exit") == 0) {

        free(city);
        return NULL; // Exit condition
    }

    return city;
}

// Function to handle numeric input
float numeric_input(const char* message) {

    float value;

    while (1) {

        printf("%s", message);

        if (scanf("%f", &value) == 1) {

            getchar(); // Consume the newline left by scanf
            return value;

        } else {

            printf("Invalid input. Please enter a numeric value.\n");
            getchar(); // Consume the invalid input
        }
    }
}

// Function for calculating drought risk
int droughtrisk(float pret_devi, float co2, char* city) {

    if (pret_devi > 5) {

        return 1; // High drought risk

    } else {

        return 0; // Low drought risk
    }
}

// Function for calculating flood risk
int floodrisk(float pret_devi, float co2, char* city) {

    if (pret_devi < -5) {

        return 1; // High flood risk

    } else {

        return 0; // Low flood risk
    }
}

// Function for calculating heatwave risk
int heatwaverisk(float temp_devi, char* city) {

    if (temp_devi > 5) {

        return 1; // High heatwave risk

    } else {

        return 0; // Low heatwave risk
    }
}

// Function for calculating air quality risk
int airquality(float AQI) {

    if (AQI > 90) {

        return 1; // Poor air quality

    } else {

        return 0; // Good air quality
    }
}

// Function to display the risks
void display(int drought, int flood, int heatwave, int air) {

    printf("\nClimate Change Risks:\n");
    printf("--------------------------------\n");

    if (drought) {

        printf("Drought Risk: High\n");

    } else {

        printf("Drought Risk: Low\n");
    }

    if (flood) {

        printf("Flood Risk: High\n");

    } else {

        printf("Flood Risk: Low\n");
    }

    if (heatwave) {

        printf("Heatwave Risk: High\n");

    } else {

        printf("Heatwave Risk: Low\n");
    }

    if (air) {

        printf("Air Quality Risk: Poor\n");

    } else {

        printf("Air Quality Risk: Good\n");
    }
}

// Function to display geographical location impacts
void DisplayGeographicalLocation(char city[50]) {
    // Accurate data for each city
    if (strcasecmp(city, "Lahore") == 0) {
        printf("Lahore is located in the Punjab region of Pakistan, near the Ravi River.\n");
        printf("Geographical Effect: Lahore experiences extreme heat in the summer with temperatures often reaching 40°C and higher.\n");
        printf("The city is vulnerable to flooding during the monsoon season due to the proximity of the Ravi River.\n");
        printf("Lahore is also affected by air pollution, particularly in winter, when fog and smog become a concern.\n");
        printf("In recent years, Lahore has faced water shortages due to over-extraction and inadequate infrastructure.\n");
    } 
    else if (strcasecmp(city, "Karachi") == 0) {
        printf("Karachi is located on the southern coast of Pakistan along the Arabian Sea.\n");
        printf("Geographical Effect: Karachi's coastal location exposes it to the threat of cyclones and sea-level rise.\n");
        printf("The city is also at risk of flooding during heavy rainfall due to inadequate drainage systems.\n");
        printf("Karachi faces extreme temperatures in summer, with temperatures often surpassing 40°C, and it experiences humid conditions due to the sea.\n");
        printf("Water scarcity is a major issue, compounded by a large population and limited water sources.\n");
    }
    else if (strcasecmp(city, "Islamabad") == 0) {
        printf("Islamabad is the capital of Pakistan, located in the foothills of the Margalla Hills, in northern Pakistan.\n");
        printf("Geographical Effect: Islamabad has a moderate climate, with cooler temperatures compared to other cities due to its higher elevation (about 540 meters).\n");
        printf("It faces fewer extreme weather conditions, but the city is vulnerable to landslides during heavy rains due to its mountainous terrain.\n");
        printf("Islamabad experiences air pollution in the winter months, mainly from vehicular emissions.\n");
        printf("The region is at risk of natural disasters like earthquakes, as it lies near the active tectonic boundary.\n");
    }
    else if (strcasecmp(city, "Rawalpindi") == 0) {
        printf("Rawalpindi is located near Islamabad, in the Punjab province, at the base of the Margalla Hills.\n");
        printf("Geographical Effect: Rawalpindi shares a similar climate with Islamabad, experiencing moderate temperatures due to its higher elevation.\n");
        printf("The city is vulnerable to flooding and landslides, particularly in low-lying areas and near the Rawal Dam.\n");
        printf("Air pollution is also an issue in the city, especially in winter.\n");
        printf("Due to its proximity to Islamabad, Rawalpindi experiences similar risks from earthquakes and extreme weather events.\n");
    }
    else if (strcasecmp(city, "Multan") == 0) {
        printf("Multan is located in the southern part of Punjab, near the Sutlej River.\n");
        printf("Geographical Effect: Multan has an arid climate with extremely hot summers, often exceeding 45°C.\n");
        printf("The city is vulnerable to seasonal flooding during the monsoon period, especially due to the proximity to the Sutlej River.\n");
        printf("Multan faces water scarcity and has been affected by the depletion of underground water reserves.\n");
        printf("Dust storms are a common occurrence in the summer, and the city suffers from poor air quality.\n");
    }
    else if (strcasecmp(city, "Faisalabad") == 0) {
        printf("Faisalabad is situated in central Punjab, near the Ravi River.\n");
        printf("Geographical Effect: Faisalabad experiences extremely hot summers with temperatures often exceeding 40°C.\n");
        printf("The city is at risk of flooding during the monsoon season, especially in low-lying areas near the Ravi River.\n");
        printf("It has an agricultural landscape, which is prone to water scarcity due to over-extraction and irregular rainfall patterns.\n");
        printf("Air quality is a significant issue due to industrial pollution and vehicle emissions.\n");
    }
    else if (strcasecmp(city, "Gilgit") == 0) {
        printf("Gilgit is located in the northern mountainous region of Pakistan, in Gilgit-Baltistan.\n");
        printf("Geographical Effect: Gilgit has a cool temperate climate due to its high elevation (about 1,500 meters).\n");
        printf("It faces the threat of floods caused by glacial lake outbursts (GLOFs), which can result in massive flash floods.\n");
        printf("Gilgit is also prone to landslides during the summer months, especially after heavy rainfall.\n");
        printf("The region is experiencing the effects of climate change, with glaciers melting faster and altering local water availability.\n");
    }
    else if (strcasecmp(city, "Quetta") == 0) {
        printf("Quetta is located in the Balochistan province, surrounded by mountains.\n");
        printf("Geographical Effect: Quetta has a dry, continental climate with cold winters and hot summers.\n");
        printf("Water scarcity is a major issue in Quetta, and it faces increasing drought conditions due to decreasing rainfall and rising temperatures.\n");
        printf("The city is also located in an earthquake-prone region due to its proximity to the active tectonic plates.\n");
        printf("Quetta faces air quality issues during the winter, mainly due to heating needs in a region with limited resources.\n");
    }
    else if (strcasecmp(city, "Peshawar") == 0) {
        printf("Peshawar is located in the Khyber Pakhtunkhwa province, near the Khyber Pass.\n");
        printf("Geographical Effect: Peshawar experiences hot summers with temperatures exceeding 40°C.\n");
        printf("Flooding is a risk, particularly in low-lying areas near the Kabul River.\n");
        printf("Air pollution is a growing concern, with smog and dust storms being frequent in winter.\n");
        printf("The city is vulnerable to climate change impacts such as reduced rainfall and increasing heatwaves, which affect agriculture and water supply.\n");
    }
    else if (strcasecmp(city, "Hyderabad") == 0) {
        printf("Hyderabad is located in the Sindh province, near the Indus River.\n");
        printf("Geographical Effect: Hyderabad faces an arid climate with very hot summers, with temperatures often exceeding 45°C.\n");
        printf("The city is at risk of flooding due to the Indus River, particularly during heavy monsoon rains.\n");
        printf("Water scarcity is a significant issue, and the city suffers from poor water management practices.\n");
        printf("Hyderabad is vulnerable to the effects of climate change, including increased heatwaves and unpredictable rainfall patterns.\n");
    } 
    
}

// Function to display geographical effect on global warming
void DisplayGeographicalEffect(char city[50]) {
    // Global warming and climate change impacts for each city
    if (strcasecmp(city, "Lahore") == 0) {
        printf("Displaying geographical impact on global warming for Lahore...\n");
        printf("Lahore is experiencing rising temperatures, with summer temperatures now regularly exceeding 40°C.\n");
        printf("Global warming has exacerbated the severity of heatwaves, impacting public health and agriculture.\n");
        printf("Air pollution levels have increased due to vehicular emissions and industrial activities, leading to smog and reduced air quality.\n");
        printf("Lahore is facing water scarcity due to declining groundwater levels and increasing demand for water.\n");
        printf("Climate change has led to unpredictable rainfall patterns, causing both droughts and floods, which affect agriculture and water resources.\n");
    } 
    else if (strcasecmp(city, "Karachi") == 0) {
        printf("Displaying geographical impact on global warming for Karachi...\n");
        printf("Karachi is experiencing more extreme heat waves, with temperatures regularly exceeding 40°C.\n");
        printf("Global warming is contributing to rising sea levels, increasing the risk of coastal flooding in low-lying areas.\n");
        printf("The city is also facing more frequent cyclones and storms, which threaten infrastructure and disrupt daily life.\n");
        printf("Water scarcity is a growing issue, with the city's demand for water surpassing available supplies.\n");
        printf("Karachi suffers from air pollution, primarily from industrial emissions and traffic, which worsens during summer heatwaves.\n");
    }
    else if (strcasecmp(city, "Islamabad") == 0) {
        printf("Displaying geographical impact on global warming for Islamabad...\n");
        printf("Islamabad is witnessing increased temperatures, although its elevation helps mitigate some effects compared to other cities.\n");
        printf("Global warming has caused erratic rainfall patterns, resulting in periodic water shortages and threats to agriculture.\n");
        printf("The city is at risk of more frequent and severe landslides due to heavy rainfall in the surrounding hilly areas.\n");
        printf("Air quality has worsened, especially in winter when pollution from heating and traffic combines with fog.\n");
        printf("Climate change may also lead to increased risk of extreme weather events like heatwaves and heavy storms.\n");
    }
    else if (strcasecmp(city, "Rawalpindi") == 0) {
        printf("Displaying geographical impact on global warming for Rawalpindi...\n");
        printf("Rawalpindi, like Islamabad, is experiencing higher temperatures, with summers becoming increasingly harsh.\n");
        printf("Flooding risks have increased due to global warming, especially in areas near rivers and low-lying regions.\n");
        printf("Water resources are under pressure due to climate change, with prolonged dry spells and unpredictable monsoon rains affecting water availability.\n");
        printf("Air pollution, especially during winter, is a growing concern due to traffic emissions and industrial activity.\n");
        printf("Climate change could lead to more extreme weather events, including floods and heatwaves, that threaten public health and infrastructure.\n");
    }
    else if (strcasecmp(city, "Multan") == 0) {
        printf("Displaying geographical impact on global warming for Multan...\n");
        printf("Multan is facing extreme heat due to global warming, with temperatures reaching 45°C or higher during summer.\n");
        printf("Global warming is exacerbating water scarcity issues, as groundwater levels continue to decline.\n");
        printf("The city is experiencing erratic rainfall patterns, with some years experiencing prolonged droughts while others face heavy flooding.\n");
        printf("Air quality in Multan is a significant concern, with dust storms and pollution peaking in the summer months.\n");
        printf("Climate change is threatening agriculture, especially crops reliant on predictable rainfall patterns, which are now increasingly unpredictable.\n");
    }
    else if (strcasecmp(city, "Faisalabad") == 0) {
        printf("Displaying geographical impact on global warming for Faisalabad...\n");
        printf("Faisalabad is experiencing hotter summers, often surpassing 40°C, which affects both agriculture and human health.\n");
        printf("Global warming has made water availability more unpredictable, leading to both droughts and flooding risks.\n");
        printf("Air pollution in Faisalabad, primarily from industries, is worsened by heat, making the city’s air quality even more hazardous.\n");
        printf("Climate change is putting pressure on the agricultural sector, particularly wheat and cotton crops, which are sensitive to temperature changes.\n");
        printf("The city is increasingly vulnerable to extreme weather events such as heatwaves, dust storms, and unseasonal rainfall.\n");
    }
    else if (strcasecmp(city, "Gilgit") == 0) {
        printf("Displaying geographical impact on global warming for Gilgit...\n");
        printf("Gilgit is experiencing a faster rate of glacial melt due to global warming, contributing to flooding risks in the valley.\n");
        printf("Global warming is leading to more unpredictable weather patterns, including irregular rainfall and changing snowmelt cycles that affect water availability.\n");
        printf("The region is at risk of more frequent natural disasters such as landslides, floods, and avalanches due to changing climate conditions.\n");
        printf("Air quality in Gilgit remains relatively better compared to other cities, but rising temperatures could threaten the region's pristine environment.\n");
        printf("As glaciers melt, the availability of fresh water will become more uncertain, threatening agriculture and local ecosystems.\n");
    }
    else if (strcasecmp(city, "Quetta") == 0) {
        printf("Displaying geographical impact on global warming for Quetta...\n");
        printf("Quetta is experiencing warmer winters and hotter summers due to global warming.\n");
        printf("Water availability is a major concern, as the region experiences prolonged droughts, affecting both agriculture and daily life.\n");
        printf("The city faces an increasing risk of extreme weather events, including heatwaves, dust storms, and flash floods.\n");
        printf("Air quality is worsened by the burning of fossil fuels, especially in winter when people use coal for heating.\n");
        printf("Climate change is affecting agriculture, particularly crops like wheat and barley, which require specific rainfall patterns.\n");
    }
    else if (strcasecmp(city, "Peshawar") == 0) {
        printf("Displaying geographical impact on global warming for Peshawar...\n");
        printf("Peshawar is experiencing increased heat during summers, with temperatures often reaching above 40°C.\n");
        printf("Global warming is contributing to reduced water supply from the Kabul River, which impacts both drinking water and irrigation.\n");
        printf("The city faces more frequent flooding, particularly in low-lying areas, and extreme heatwaves that affect public health.\n");
        printf("Air pollution is worsening in Peshawar, driven by emissions from vehicles and industrial activity, leading to smog, especially in winter.\n");
        printf("Climate change is altering rainfall patterns, affecting local agriculture, particularly in areas that rely on consistent seasonal rainfall.\n");
    }
    else if (strcasecmp(city, "Hyderabad") == 0) {
        printf("Displaying geographical impact on global warming for Hyderabad...\n");
        printf("Hyderabad is experiencing hotter summers with temperatures regularly exceeding 45°C.\n");
        printf("The city faces water scarcity due to increasing temperatures, reduced rainfall, and over-extraction of groundwater.\n");
        printf("Flooding is a risk, especially during monsoons, due to poorly managed drainage systems and the proximity to the Indus River.\n");
        printf("Air pollution is worsening in Hyderabad, primarily from industrial activities and vehicular emissions, contributing to smog and health risks.\n");
        printf("Climate change has also led to more frequent heatwaves, threatening agriculture and local ecosystems.\n");
    } 
    
}
void timer(){
	printf("\n\n\n\n\n\n\n\n\nThe Output is being processed Please Wait.");
	for(int i=9;i>=0;i--){
		printf("%d",i);
	    sleep(1);
		printf("\b");
	}
	system("cls");
}
// Function to save data to a file
void save_to_file(char* city, float cutemp, float cupret, float AQI, float co2, 
float avgtemp, float avgpret, int drought, int flood, int heatwave, int air) {
    FILE *file = fopen(FILE_NAME, "a");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    fprintf(file, "\nCity: %s\n", city);
    fprintf(file, "Current Temperature: %.2f°C\n", cutemp);
    fprintf(file, "Current Precipitation: %.2f%%\n", cupret);
    fprintf(file, "Air Quality Index: %.2f\n", AQI);
    fprintf(file, "CO2 Emission Rate: %.2f tons/year\n", co2);
    fprintf(file, "Average Temperature (Seasonal): %.2f°C\n", avgtemp);
    fprintf(file, "Average Precipitation (Seasonal): %.2f%%\n", avgpret);
    fprintf(file, "Drought Risk: %d\n", drought);
    fprintf(file, "Flood Risk: %d\n", flood);
    fprintf(file, "Heatwave Risk: %d\n", heatwave);
    fprintf(file, "Air Quality Risk: %d\n", air);
    fclose(file);
}
