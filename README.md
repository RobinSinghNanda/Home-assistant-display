# Home-assistant-display

A Home Assistant display based on ESP32. It uses Home Assistant's official Websockets API to fetech and upload states of the entites. The screens can be configured using lovelace's card json (yml converted to json).

Example mainscreen configuration with a single page. The name and icon can be automatically feteched from Home Assistant if not specfied. 
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
Screen samples captured from hardware.<br />
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/raw/main/images/main_screen.png?raw=true)
![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/blob/main/images/sensors_screen.png?raw=true)

