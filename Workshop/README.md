
# Word Guessing Game with Flask

# Server.py

A simple web-based word guessing game implemented using Flask. Players attempt to guess the word "python" or any specified word letter by letter, with a maximum of 6 incorrect attempts allowed.

## Features:
1. Players guess letters to reveal the hidden word.
2. The game provides feedback on each guess, informing players if they've already guessed a letter or if their guess is incorrect.
3. Players can view the letters they've guessed and the number of attempts they have left.
4. The game ends when players either guess the word or exhaust their allowed attempts.

## Endpoints:

1. **/winner**: Serves a template congratulating the player for winning.
2. **/looser**: Serves a template showing the game-over message for losing.
3. **/game**: The main game interface. 
    - GET: Displays the current game status.
    - POST: Processes a player's letter guess.
4. **/game_status**: Returns a JSON representation of the game's status.
5. **/status**: Returns a simple JSON message indicating the connection status.

## Getting Started:

### Prerequisites:
Ensure you have Flask and Flask-CORS installed:
```bash
pip install Flask Flask-CORS
```

### Running the Game:
1. Clone the repository.
2. Navigate to the directory containing the game script.
3. Run the Flask application:
```bash
python server.py
```
4. Open your browser and navigate to `http://127.0.0.1:5000/game` to start playing!

# client.py 

A simple Python client that allows players to interact with a web-based word guessing game. This client connects to a Flask-based game server, allowing players to guess the word letter by letter, with feedback on each guess.

## Features:

1. Players can input letters to guess the word.
2. After each guess, the client fetches the current game state from the server and displays:
    - The word as it is currently revealed.
    - The number of incorrect attempts left.
    - The letters already guessed.
    - Feedback messages on the latest guess.

## How it Works:

The client sends HTTP requests to the Flask-based game server:
- A **POST** request to `/game` with the guessed letter.
- A **GET** request to `/game_status` to fetch the current state of the game.


## Prerequisites:

Ensure you have the `requests` library installed:
```bash
pip install requests
```

Change the IP address to your server

### Running the Client:

1. Clone the repository.
2. Navigate to the directory containing the client script.
3. Run the client:
```bash
python client.py
```
4. Follow the prompts to guess letters and reveal the word!

# cient.html

An interactive frontend interface for a word-guessing game. Built using vanilla JavaScript and HTML, this web page allows players to guess letters and see the current status of the game in real-time.

## Features:

1. **Letter Input**: Players can input a letter to guess the word.
2. **Real-time Feedback**: After every guess, the game's current status is fetched from the server and displayed on the page, including:
   - The word as it is currently revealed.
   - The number of incorrect attempts left.
   - The letters already guessed.
   - Feedback messages on the latest guess.


## Usage:

1. Open the `index.html` (or whatever the name of the HTML file is) in your preferred web browser.
2. Enter a letter in the input box and click "Guess".
3. The game status will update immediately, showing you the current state of the game.

## Technical Details:

- The JavaScript code utilizes the `fetch` API to send POST requests to the `/game` endpoint for submitting letter guesses.
- After submitting a guess, the game status is fetched from the `/game_status` endpoint.
- The game status is then displayed dynamically on the page using DOM manipulation.

## Customizing the Server Address:

By default, the frontend is set to communicate with a backend server at `http://192.168.1.198:5000`. If your backend server is hosted at a different address, update the `base_url` variable in the JavaScript section of the HTML file.


