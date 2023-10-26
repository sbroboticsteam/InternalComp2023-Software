import requests

base_url = "http://127.0.0.1:5000"
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


# response = requests.get(f"{base_url}/status")
# print(response.status_code)
# data = response.json()
# print(data['connection'])