import requests

base_url = ""
game_url = f"{base_url}/game"
status_url = f"{base_url}/game_status"

while True:
    letter = input("Guess a letter: ")
    # -- Your Work ---
    # Add a post request here
    # response =                                # get the status here
    # ==========================================================

    data = response.json()
    print("Word:", data["word_display"])
    print("Attempts left:", data["attempts_left"])
    print("Guessed letters:", ', '.join(data["guessed_letters"]))
    print(data["message"])
    print("-" * 50)