import requests

base_url = "http://10.1.221.244:5000/"
game_url = f"{base_url}/game"
status_url = f"{base_url}/game_status"

while True:
    letter = input("Guess a letter: ")
    requests.post(game_url, data={"letter": letter})
    response = requests.get(status_url)

    data = response.json()
    print("Word:", data["word_display"])
    print("Attempts left:", data["attempts_left"])
    print("Guessed letters:", ', '.join(data["guessed_letters"]))
    print(data["message"])
    print("-" * 50)

