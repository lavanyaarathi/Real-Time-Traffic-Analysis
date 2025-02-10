from flask import Flask, request, jsonify
import subprocess

app = Flask(__name__)

@app.route('/shortest_path', methods=['POST'])
def get_shortest_path():
    data = request.get_json()
    start = data['start']
    end = data['end']

    # Run the C++ executable with start and end as arguments
    result = subprocess.run(['./traffic_analysis.exe', str(start), str(end)], capture_output=True, text=True)

    # Capture the output
    output = result.stdout
    
    # Format the output to JSON
    response = {"path": output}
    return jsonify(response)

if __name__ == "__main__":
    app.run(debug=True)
