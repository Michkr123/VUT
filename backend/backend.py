from flask import Flask, request, jsonify
from flask_cors import CORS
import json
from datetime import datetime
import base64

app = Flask(__name__)
CORS(app)

def is_valid_base64_image(base64_string):
    try:
        if not base64_string.startswith('data:image'):
            return False
        format, imgstr = base64_string.split(';base64,')
        base64.b64decode(imgstr)
        return True
    except Exception:
        return False


def load_events():
    try:
        with open('events.json', 'r') as file:
            return json.load(file)
    except FileNotFoundError:
        print('events.json not found')
        return []
    
def load_profiles():
    try:
        with open('profiles.json', 'r') as file:
            return json.load(file)
    except FileNotFoundError:
        print('profiles.json not found')
        return []


def save_events(events):
    with open('events.json', 'w') as file:
        json.dump(events, file, indent=4)


def load_reviews():
    try:
        with open('reviews.json', 'r') as file:
            return json.load(file)
    except FileNotFoundError:
        print('reviews.json not found')
        return []


def save_reviews(reviews):
    with open('reviews.json', 'w') as file:
        json.dump(reviews, file, indent=4)


def calculate_average_rating(event_id, reviews):
    event_reviews = [review for review in reviews if review['event_id'] == event_id]
    if not event_reviews:
        return None
    return sum(review['rating'] for review in event_reviews) / len(event_reviews)


@app.route('/events', methods=['GET'])
def get_events():
    events = load_events()
    reviews = load_reviews()
    
    # List události obohacený o recenze
    enhanced_events = []
    for event in events:
        event_copy = event.copy()

        # Je u události obrázek?
        if 'image' in event_copy:
            event_copy['image'] = True 
        else:
            event_copy['image'] = False
            
        # Kalkulace průměrného hodnocení akce a počtu recenzí
        event_reviews = [review for review in reviews if review['event_id'] == event['id']]
        event_copy['average_rating'] = (
            sum(review['rating'] for review in event_reviews) / len(event_reviews)
            if event_reviews else None
        )
        event_copy['review_count'] = len(event_reviews)
        
        enhanced_events.append(event_copy)
    
    return jsonify(enhanced_events)

@app.route('/profiles/<login>', methods=['GET'])
def get_profile(login):
    profiles = load_profiles()
    profile = next((p for p in profiles if p['login'] == login), None)
    if profile is None:
        return jsonify({"error": "Profile not found"}), 404
    return jsonify(profile)


@app.route('/events', methods=['POST'])
def add_event():
    events = load_events()
    event_data = request.get_json()
    
    # Kontrola obrázku pokud byl poskytnut
    image_data = event_data.get("image")
    if image_data and not is_valid_base64_image(image_data):
        return jsonify({"error": "Invalid image format. Must be base64 encoded image."}), 400

    event = {
        "id": len(events) + 1,
        "name": event_data.get("name"),
        "date_of_creation": datetime.now().isoformat(),
        "available_tickets": event_data.get("available_tickets"),
        "date_of_event": event_data.get("date_of_event"),
        "event_category": event_data.get("event_category"),
        "address": event_data.get("address"),
        "image": image_data
    }
    events.append(event)
    save_events(events)
    return jsonify(event), 201


@app.route('/events/<int:event_id>/image', methods=['GET'])
def get_event_image(event_id):
    events = load_events()
    event = next((event for event in events if event['id'] == event_id), None)
    if not event:
        return jsonify({"error": "Event not found"}), 404
    if not event.get('image'):
        return jsonify({"error": "No image found for this event"}), 404
    return jsonify({"image": event['image']})


@app.route('/events/<int:event_id>/image', methods=['PUT'])
def update_event_image(event_id):
    events = load_events()
    event_index = next((index for index, event in enumerate(events) if event['id'] == event_id), None)
    if event_index is None:
        return jsonify({"error": "Event not found"}), 404

    image_data = request.get_json().get('image')
    if not image_data:
        return jsonify({"error": "No image provided"}), 400
    if not is_valid_base64_image(image_data):
        return jsonify({"error": "Invalid image format. Must be base64 encoded image."}), 400

    events[event_index]['image'] = image_data
    save_events(events)
    return jsonify({"message": "Image updated successfully"})


@app.route('/events/<int:event_id>/reviews', methods=['GET'])
def get_event_reviews(event_id):
    reviews = load_reviews()
    event_reviews = [review for review in reviews if review['event_id'] == event_id]
    return jsonify(event_reviews)


@app.route('/events/<int:event_id>/reviews', methods=['POST'])
def add_review(event_id):
    
    events = load_events()
    event_exists = any(event['id'] == event_id for event in events)
    if not event_exists:
        return jsonify({"error": "Event not found"}), 404

    reviews = load_reviews()
    review_data = request.get_json()

    rating = review_data.get("rating")
    if not isinstance(rating, (int, float)) or not (1 <= rating <= 5):
        return jsonify({"error": "Rating must be between 1 and 5"}), 400

    review = {
        "id": len(reviews) + 1,
        "event_id": event_id,
        "username": review_data.get("username"),
        "comment": review_data.get("comment"),
        "rating": rating,
        "date_posted": datetime.now().isoformat()
    }

    reviews.append(review)
    save_reviews(reviews)
    return jsonify(review), 201


@app.route('/events/<int:event_id>', methods=['GET'])
def get_event_with_reviews(event_id):
    events = load_events()
    reviews = load_reviews()
    
    event = next((event for event in events if event['id'] == event_id), None)
    if not event:
        return jsonify({"error": "Event not found"}), 404

    event_reviews = [review for review in reviews if review['event_id'] == event_id]
    
    if event_reviews:
        avg_rating = sum(review['rating'] for review in event_reviews) / len(event_reviews)
    else:
        avg_rating = None

    response = {
        **event,
        "reviews": event_reviews,
        "average_rating": avg_rating,
        "review_count": len(event_reviews)
    }
    
    return jsonify(response)


if __name__ == '__main__':
    app.run(debug=True)
