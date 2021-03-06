# Game Design Group 3 Spring 2018
Project repo for Chaos Castle, a co-op game for Android (and iOS).

## Final release notes
Chaos Castle (single player) requires no additional configuration. Build and run this project as you would 'HelloWorld' for Android, Apple, and Windows.

### Changes from Open Beta
The mage room can cast AOE spells on a single wall when it's unlocked from level 8 onwards. To cast a spell, tap a side of the castle (i.e. black hexagon) you wish to cast the spell at. Then tap the spell you wish to cast:

Fire-deal 1 damage to all enemies on a side. Great for taking out hordes of weak enemies.

Blizzard-slow down all enemies on a side, frozen enemies take double damage from arrows. Great for taking down strong targets.

Shield-Prevent damage for a specific wall for a few seconds; great if your oil is still recharging.

### Updated mutliplayer instructions
To set up mutliplayer, two Android devices need to be paired up beforehand (through your phone settings.) Afterward, tap "mutliplayer" and "create room" for one of the devices. Afterwards, the other player should tap on the host's device name. Both players will then be taken to the mutliplayer endless mode, where all rooms except for the mage are unlocked.


## Open Beta release notes

### Changes from Closed Beta
Multiplayer works for up to six players! All devices must run Android. Before starting the game the server device (the one to create the room) must have previously paired with all client devices via Bluetooth (i.e. have their Bluetooth ID saved). Once the server creates a room, it may take a few seconds for the room to display on the clients' lobby screen.

The mage room is now online, however spells effects are not yet drawn/implemented. Create gestures with your finger within the hexagon in the mage room (or as mentioned in the beta feedback, the "ink room") to cast a spell. Tap a side of the hexagon to apply that spell to that room.

We recalibrated the lookout scene so that enemies aren't closer than they appear.

### Roadmap
The server must be recognize all the clients as previously paired devices before the game starts. We are working to enable discoverability, though this will not be an issue at showcase since we'll be demo-ing with our own devices.

Spell effects have yet to be implemented, so the mage room has no gameplay inpact yet.

We don't have attack animations yet for the berserker (red reaper), warrior (small reaper), and the big reaper. However once they approach the castle, they will still continuously damage the wall.

We're currently working on implementing indicators for when your castle is being damaged from a particular side.


## Closed Beta release notes

### Changes from Alpha
All oil buttons are fully functional. Tilt your phone forward to spill oil and destroy all enemies in that direction. On Windows, hold the up arrow to simulate tilting forward; release to tilt back. Enemies that disappear from the range of the ballista appear in the oil scene.

The first four levels are implemented and can be accessed through the level select menu.

Flying enemies stay a distance away from the castle and attack at a range once they stop moving forward.

### Networking
This branch contains no networking functionality. Ashton's and Yiming's networking code is on a parallel branch and we'll demo to Walker on Monday.

Establishing bluetooth connections are tested and proven to work; we are currently debugging sending and interpreting game states over the connection.


### Known Issues
The flying enemies are missing an attack animation, so it looks like the castle is taking chip damage (every 75 frames) once they stop moving.

Ballista, oil, and enemy art assets are jittery because the animation strips are not perfectly aligned; this is not because of physics.


## Alpha release notes

### Changes and Notes
Only the **north** oil button will take you to the oil scene. This is because we are planning to refactor how we consider directions in the Beta release.

Fire the ballista by dragging anywhere on the screen in the opposite direction.

In the Ammunition Room, tap the large ammo button in the center to make arrows, the closer the slider gets to the center, the more arrows you make (think of how you attack in Undertale). The slider does move, it just has a slow start.


### Known Issues
Working bluetooth code is on a separate development branch that Ashton has demoed separately to Walker. **Do not click the "Lobby" button on Android builds** as it will crash the app.

We are getting rid of the catapult role, though the castle still has a catapult floor as the art assets have not yet been updated.


## Git Usage

Create a new branch for each coding task, and name the branch \[netId\]\_\[codingTask\].

When you're done with your coding task, submit a **pull request**. We'll merge our changes to the master branch during coding meetings or after class.

Keep commit messages under 80 characters. Commit messages should complete this sentence: "If merged, this commit will \_\_\_\_\_\_\_\_\_\_\_\_". Ex. `git commit -m "add art assets for castle"` `git commit -m "implement basic networking"`


## Cornell University Game Library (CUGL)

This template provides a base for you to build off of.  You should add all of your
files to sources and assets, as appropriate.  This will allow access to the Apple,
Android, and Windows IDEs.

You will need to make changes to the project to give it a different name and package
identifier (to prevent your application from clashing with others).  See the course 
web page for instructions
