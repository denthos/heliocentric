## Group 2 Space Colony Game Idea ##
Here is a list of some basic specifications of the game as I imagine it looking. Please provide feedback and suggest changes!  - Dylan


----------


**Controls**

Input Devices

 - Mouse
  - The mouse is able to select game objects, move the camera around the game world, as well as interact with the interface.
  - Left clicking an object should select it, and the interface should change to show the player the options available for interacting with that unit as well as information about it.
  - Moving the camera is a bit tricky in a 3D space where you can go anywhere, so it has its own section below.
  - Interacting with the interface should be pretty simple, left clicking buttons performs that buttons relevant action.
 - Keyboard
  - The keyboard is used for shortcuts and customizable keybindings as an alternative to clicking interface items. Potentially also uses strafe controls (see below).

Movement Options

 - Free Movement Controls
  - The player uses some combination of keys (typically WASD) to handle movement via strafing, with the keys possibly changing function while the left mouse button is held.
  - The player moves the mouse while holding the left mouse button to rotate the camera within some constraints.
  - This would be difficult to find and see objects as they all move around the space at different speeds, but lets the player have total freedom of where to look. (problems here are disguising things with fog of war if we have that mechanic)

 - Orbital Movement Controls
  - The camera stays fixed on the object selected or the planet that the object is located on as the object itself moves through space.
  - The player is able to move the camera around the selected object by holding a mouse button and moving the mouse. In this way the player could select a planet or something on a planet and then rotate around the planet to see everything on it. Alternatively, selecting a spaceship or something similar would cause the camera to follow that.
  - The scroll wheel would be used to get farther and closer to the selected object.
  - This option could be a bit clunky to see other things in space and select new objects properly as they move in relation to one another, but allows for a much smoother viewing experience as you don't have to worry about objects moving away from your view.

There are pros and cons to both movement options, and we could definitely just let the player switch between them as they see fit. If anyone has alternative movement option ideas, please share them!

----------
**Main Menu**

 - Host Game
  - This menu item should take the player directly to a lobby in which they are the host
 - Join Game
  - This menu item should prompt the player for the method with which he wishes to connect to a game lobby (these options will depend on how we choose to handle networking)
 - Settings
  - This menu item should take the player to the settings screen. This should initially include a resolution option, and the player's desired screen name. We can add further options as we develop.
 - Exit Game

----------
**Lobby Screen**

The Lobby screen should show a list of the players currently connected, a button to change faction/race/empire/other, and a button to view the game settings. As we develop we may want to add a chat window so that players can communicate.

- Player List
 - The player list will list all of the players currently connected to the lobby as well as relevant information about them. This should initially include their chosen name, their chosen color, and their currently selected faction/race/empire/other. We can expand this to include things like latency, and the ability for players to change their color after we finish the core game.
- Faction Change Button
 - The faction change button should open a small prompt in which the player can select their faction. Initially we should limit the number of factions to one, but can add more as a stretch goal.
- Game Settings Button
 - This button should open a window that displays the currently selected game settings. If the player is currently the host, these settings should be editable. Alternatively this can simply be an additional window that is always displayed in the lobby screen.
 - Game Settings can include things like number of planets, starting resources, scarcity of resources, solar system density (for things like asteroids, moons, etc), time limit, and other options that depend on the world generation and other game specifications. These options should have reasonable minimum and maximum values as constraints (e.g. require that number of planets at a minimum be the number of players present, etc). In the future we could add options to enable or disable certain victory conditions, among other interesting options.
- Chat Window (maybe optional)
 - Don't fix what isn't broken. Something simple like how SC2 does it, a simple box where you can type a message and press Enter to send it to all players in the lobby, see messages from other players, and important information such as players leaving and entering the lobby. See [this image](http://i.imgur.com/0qt2jnG.jpg).
 
 
 ----------
 **Networking Schema**

There are a few options for how to approach the networking that might drastically change how we handle the game logic. The two most prominent options are using P2P between the clients, or having a server-client relationship with a trusted source. With P2P, updating what is happening in the game locally is a bit messy because of the asynchronous nature of inputs received, and the client code will be more involved, typically.

- P2P Model
 - This gets a little tricky but can be pretty interesting to implement. Each client sends its actions to each other as they are performed. Each client attempts to determine what is happening based on its own actions and the actions that it is receiving from the other clients and they all try to sync entire game states periodically to correct for any errors that are happening. I'm assuming we aren't too worried about people cheating and sending false data, so we shouldn't have to worry about determining trust by comparing the different received game states and stuff like that. This is definitely a difficult option, but allows us to only have to write clients, and no additional server.
 - There are absolutely other ways to implement P2P, these are just initial thoughts based on how some other games do things.
- Server - Client Model
 - The host of the game (upon lobby creation) starts up another process that is the server program. This program receives data from all of the clients, and sends data out to them. It acts as the trusted source for the current game state. This will likely be far simpler.
  - Again, there are totally other ways to do things, feel free to share ideas with the group!

---------
**Game Specs**

Play Area

- The play area will consist of a single solar system, randomly generated upon game start. The solar system should include, at the minimum, planets of varying size and with differing resources orbiting a central star. We can expand this to include asteroids, moons, and other celestial bodies.

Solar System Generation

- The solar system should be generated randomly within parameters set by the host of the game (see lobby screen section for details). The celestial bodies should be placed on non-intersecting paths with variable orbiting velocities, and with variable sizes and resources available. The resource placement should be balanced in some way so no player starts with a significant lead.

Resources and Currency

- There should be a variable number of resources that are used to build and research
 - Some examples are iron, uranium, iridium, energy (generic fuel, a planets energy supplies might come in the form of natural gases, oils, atmospheric conditions for wind and such, or exposure to the sun), along with many others
 - Resources are gained by building resource extraction structures on celestial bodies. For simplicity, this could be in the form of simple upgrades to cities, and your initial cities have basic resource extraction already researched. As a stretch goal, they could be standalone structures that can be built, so larger planets allow for additional resource extraction.
- Global currency (optional)
 - Production of anything and certain research options require currency as a currency drain. This inherently gives the currency value. High value production means you need to have amassed large amounts of currency. Currency should be generated by cities based on their size and population (or other factors). Additionally, as a stretch goal, random events might introduce ways of getting extra currency.
 - Another option for global currency is that the game starts with a fixed amount, and it can be traded to other players, looted from them (assuming combat exists), or gifted as a diplomatic gesture. The value of this currency is entirely what the players make it out to be in this case.

Research and Upgrades

- Each player owned object should have a set of possible upgrades. For individual units, such as spacecrafts or others, these upgrades are determined by what the players technology tree is at when the unit is constructed. For cities or other structures, each city should be able to develop specific upgrades as well as reaping benefits from the players currently researched technology tree.
- Technology Trees
 - Science
	 - The science technology tree deals with upgrades like improved space travel, better resource extraction techniques, unlocking upgraded structures to build
 - Economic / Political
	 - The economic tree deals with upgrades like improved wealth generation, reduced costs of building, improved trade routes (potentially by unlocking better trade/diplomacy oriented units)
 - Military (assuming combat)
	 - The military tree deals with upgrades like improved combat damage, improved looting, unlocking better combat units

Exploration and Settling

- Traveling to new celestial bodies should require a spaceship, and the player should be able to set a destination for a spaceship which will then automatically travel an optimal route. Alternatively, we can allow for more flexibility in how a player's ships reaches a certain area (potentially to avoid risky zones if there is combat, Building new settlements on unsettled bodies should probably require a special, more expensive unit, or specific research in science

Travel Time

- Solar systems are big. In order to allow the players to experience the scale at hand, it should take time to travel across the system to do various things like trading, waging war or raiding, etc. This will incentivize players to upgrade their ships' speed, settle planets that are closer to allies, or farther away from enemies, and overall make the game a bit more complex.
- Solar systems are constantly changing. As celestial bodies move around, different opportunities are available. Travel times decrease, and maybe waging war on that enemy player is not feasible when it takes your ships a long time to get there. This might leave you vulnerable since you can't pull your ships back very quickly, and it also allows the enemy time to respond effectively. Tensions might get high as the planets line up, and you might get a reprieve as they drift apart. I think this adds a pretty interesting tempo to the game based on how the generated solar system behaves that is hard to prepare for ahead of time.
- Obviously we don't want the game to take forever, so getting orbits to act reasonable from a gameplay perspective will take some tuning, as will determining how fast ships travel relative to the motion of planets.
- Gravity is serious business. As a stretch goal we can simulate a simple gravity field that changes with the planets so that ships get speed bonuses when moving near planets. This might encourage forming alliances so that you can safely use ally planets to slingshot your way around without having to worry about being attacked.

Victory Conditions

- Tech Victory
 - If a player successfully researches everything in the science tree, they are able to build an intergalactic ship or structure, thus expanding even beyond the solar system, winning the game
- Elimination Victory (assuming combat)
 - If a player eliminates all other players, they win
- Expansion Victory
 - If a player controls over a certain % of the celestial bodies (potentially based on mass), they win
- Economic Victory
 - If a player manages to accumulate a certain amount of currency over the course of the game, they win
 - In this victory condition, spending currency might not reduce your total accumulated amount, but trading it away will

Diplomacy and Trading

- Options to declare war, negotiate peace (assuming combat)
 - These actions should be declared to all players
- Options to offer one time or automatically repeating trades
 - Trades must occur between player owned structures that house a trade ship with the capacity to handle the quantity being traded (maybe a stretch goal)

Units

- TODO

Structures

- TODO

Factions

 - TODO

-------
**Minimum Requirements**

- Game Lobby
- Solar System Generation
- Realtime networking
- Functional Controls
- Ability to build structures and units
- A single faction
- A fair number of units
- A fair number of upgrades
- Cities handle resource and currency generation based on planet parameters
- Simple Trade System
- Simple Combat System
 - Each space unit has an attack, armor, health
 - Structures have health, start with 0 attack but can be upgraded with guns
 - Players can only command units to attack other specific units, idle military units will attack hostile units automatically
 - No micro from the player, attacks are performed instantly despite visuals
- Victory Conditions

--------

**An Argument for Violence**

The idea that combat should be left as a stretch goal was tossed around in our initial discussion, but I believe that combat should be given just as much priority as any other piece of gameplay. In a RTS game like this, a lack of combat would remove all element of risk to the game. The victor would be decided by who has the best macro and through a small number of trades. If one player is advancing at a faster rate economically or research wise, there is no way for other players to catch up or compete, except through trading with each other solely in an attempt to defeat the currently winning player. There is no risk to expanding as soon as possible, because you don't have to build defenses. In my opinion, this strips the game of a lot of interesting decision making and risk management. Spreading your empire too thin in an attempt to accrue wealth might open you up to being conquered utterly because you can't afford to defend all of your assets. Bunker up to research as much as possible, and you will be quickly outpaced by people that expanded. Spend all of your wealth on military advancement and it is unlikely that you can compete with the raw resources and wealth of those investing in their infrastructure in the long term. The introduction of combat adds not only risk, but a balancing force to the game that forces players to engage in all aspects of the game in order to win.