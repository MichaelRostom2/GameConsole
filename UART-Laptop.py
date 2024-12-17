import serial
import time
import json

SCORE_FILE = "scores.json"
# loades scores from file
def load_scores():
    try:
        with open(SCORE_FILE, "r") as file:
            scores = json.load(file)
            print(f"Loaded scores: {scores}")
            return scores
    except (FileNotFoundError, json.JSONDecodeError):
        return {"ping": 0, "dodge": 0}

# Save scores to file
def save_scores(scores):
    with open(SCORE_FILE, "w") as file:
        json.dump(scores, file)
    print(f"Scores saved: {scores}")

# Process incoming commands
def process_command(command, scores, ser):
    print(f"Received: {command}")

    if command.startswith("CMD:UPDATE"):
        parts = command.split(" ")
        if len(parts) == 3:
            game = parts[1]
            new_score = int(parts[2])

            if new_score > scores.get(game, 0):
                scores[game] = new_score
                save_scores(scores)
                print(f"Updated {game} score to {new_score}")
            else:
                print(f"Received lower score for {game}, keeping {scores[game]}")

    elif command.startswith("CMD:GET"):
        # Parse the game
        parts = command.split(" ")
        if len(parts) == 2:
            game = parts[1]
            current_score = scores.get(game, 0)
            response = f"CMD:SCORE {game} {current_score}\n"
            ser.write(response.encode())
            print(f"Sent: {response.strip()}")
# Main function
def main():
    scores = load_scores()

    ser = serial.Serial('/dev/tty.usbmodemF412FA9F9FB02', 9600, timeout=1)  # Replace 'COM3' with your port
    time.sleep(2)

    print("Listening for Arduino...")
    try:
        while True:
            if ser.in_waiting > 0:
                command = ser.readline().decode().strip()
                if command:
                    process_command(command, scores, ser)
    except KeyboardInterrupt:
        print("Exiting...")
        save_scores(scores)
    finally:
        ser.close()
# Run the main function
if __name__ == "__main__":
    main()