# Home-assistant-display WIP

A Home Assistant display based on ESP32. It uses Home Assistant's official Websockets API to fetech and upload states of the entites. The screens can be configured using lovelace's card json (yml converted to json).

## Screen samples
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/raw/main/images/main_screen_light.png?raw=true)
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/blob/main/images/sensors_screen_light.png?raw=true)
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/blob/main/images/display_setings_light.png?raw=true)<br/>
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/raw/main/images/main_screen_themed.png?raw=true)
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/blob/main/images/sensors_screen_themed.png?raw=true)
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/blob/main/images/display_settings_themed.png?raw=true)<br/>
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/blob/main/images/light_subscreen.png?raw=true)
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/blob/main/images/light_subscreen2.png?raw=true)

## PCB images
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/blob/main/images/pcb_front.PNG?raw=true)
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/blob/main/images/pcb_back.PNG?raw=true)<br/>
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/blob/main/images/HAD_3.5_front.png??raw=true)
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/blob/main/images/HAD_3.5_back.png?raw=true)<br/>

## Lovelace card type supported.
1. [entities](https://www.home-assistant.io/lovelace/entities/)
2. [light](https://www.home-assistant.io/lovelace/light/)
3. [horizontal-stack](https://www.home-assistant.io/lovelace/horizontal-stack/)
4. [vertical-stack](https://www.home-assistant.io/lovelace/vertical-stack/)
5. [weather-forecast](https://www.home-assistant.io/lovelace/weather-forecast/)
6. [thermostat](https://www.home-assistant.io/lovelace/thermostat/)
7. [alarm-panel](https://www.home-assistant.io/lovelace/alarm-panel/)
8. [media-control](https://www.home-assistant.io/lovelace/media-control/)
9. [humidifier](https://www.home-assistant.io/lovelace/humidifier/)
10. [custom:paper-buttons-row](https://github.com/jcwillox/lovelace-paper-buttons-row)

## Sample configuration file
```json
[{
   "entities": [
      {
         "type": "custom:paper-buttons-row",
         "buttons": [
         {
            "entity": "scene.good_morning",
            "name": "Sunshine"
         },
         {
            "entity": "scene.good_night",
            "name": "Bedtime"
         },
         {
            "entity": "scene.going_out",
            "name": "Lock"
         },
         {
            "entity": "scene.i_am_home",
            "name": "Home"
         }
         ]
      },{
         "entity":"light.bedroom_light",
         "name": "Light"
      },{
         "entity": "fan.main_bedroom_fan",
         "name": "Fan",
         "icon": "mdi:fan",
	      "state_color": 0
      }, { 
         "entity":"light.main_bedroom_bed_light",
         "name": "Bed"
      }, {
         "entity": "media_player.bedroom_tv",
         "name": "TV",
	      "icon": "mdi:television"
      }
   ],
   "show_header_toggle": false,
   "title": "Bedroom",
   "icon": "mdi:home-assistant",
   "type": "entities"
}]
```




