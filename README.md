# MyApex
Simple C++ cheats for Apex Legends (Linux Steam version). 

### Sense 
Hijacks Bloodhound Scan/Caustic Vision to force enable ESP on all players (Visible and Invisible).\
Has color codes for each type of Evo shield and shows color ranges between green and red for high health and low health respectively
Has Weapon Chams / Invisible Weapon in hand.
Has thirdperson view activated by PageDown Key

### NoRecoil
Reduce weapon recoil (could be improved)


### Aimbot
Decent aimbot with adjustable FOV, smoothness, and distance.

## Requirements
1. Linux with Xorg
2. g++
3. Git

### Warnings
The chances of getting banned by EAC using this cheat are basically zero. (You have better chances of getting VAC banned on CSGO than by EAC)\
However, you can get manually banned which has happened to me on several occasions.\
You should also use an alt account.

This isn't required, but you should change the sysctl parameter *kernel.yama.ptrace_scope* to atleast 1 (preferably 2).\
This will completely disable process debugging for non-root processes which should in theory handicap EAC even further.\
You can do so with this command: ```sysctl kernel.yama.ptrace_scope=2```

### Download & Compile

```
git clone https://github.com/AdverseMiller/myapex.git
cd myapex
./build
```

4. Open the game

5. Run the cheat (needs root for memory r/w and prevent EAC detection)
```
./sh
```

The cheat will then load the config located in myapex.ini by default, you can choose a config file by providing it as a start argument.\
Example:
```
./sh otherconfig.ini
```












