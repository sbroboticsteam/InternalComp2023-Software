from flask import Flask, request, render_template, jsonify, redirect, url_for
import random

app = Flask(__name__)

WORDS = ["python", "flask", "hangman", "requests"]
word_to_guess = random.choice(WORDS)
guessed_letters = []
attempts_left = 6
message = ''

@app.route('/game', methods='POST')
def game():
    global word_to_guess, guessed_letters, attempts_left, message
    message = ''

    if request.method == 'POST':
        letter = request.form.get('letter')
        letter = str(letter)
        if letter in guessed_letters:
            message = "Letter already guessed!"
        else:
            guessed_letters.append(letter)
            if letter not in word_to_guess:
                attempts_left -= 1
            if attempts_left <= 0:
                message = f"Game over! The word was {word_to_guess}."
            word_display = ''.join([l if l in guessed_letters else '_' for l in word_to_guess])
            if "_" not in word_display:
                message = f"Congratulations! You've guessed the word: {word_to_guess}!"
        
        return redirect(url_for('game'))

    word_display = ''.join([l if l in guessed_letters else '_' for l in word_to_guess])
    return render_template('index.html', word_display=word_display, attempts_left=attempts_left, guessed_letters=guessed_letters, message=message)

@app.route('/game_status')
def game_status():
    word_display = ''.join([l if l in guessed_letters else '_' for l in word_to_guess])
    return jsonify({
            "word_display": word_display,
            "attempts_left": attempts_left,
            "guessed_letters": guessed_letters,
            "message": message
        })

if __name__ == '__main__':
    app.run(debug=True)

