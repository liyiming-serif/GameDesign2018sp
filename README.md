# Game Design Group 3 Spring 2018
Project repo for Chaos Castle, a co-op game for Android (and iOS).

## Closed Beta release notes
Chaos Castle requires no additional configuration. Build and run this project as you would 'HelloWorld' for Android, Apple, and Windows.

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
