# Home-assistant-display

A Home Assistant display based on ESP32. It uses Home Assistant's official Websockets API to fetech and upload states of the entites. The screens can be configured using lovelace's card json (yml converted to json).

Example mainscreen configuration with a single page. The name and icon can be automatically feteched from Home Assistant if not specfied. 
```json
[{
   "type": "entities",
   "entities": [
      "light.main_bedroom_light", {
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
      }
   ],
   "title": "Powerstrip6",
   "state_color": true
}]
```

![alt text](https://github.com/RobinSinghNanda/Home-assistant-display/raw/main/images/main_screen.png?raw=true)
