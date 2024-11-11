import React, { useState, useEffect } from 'react';
import { useParams } from 'react-router-dom';
import EventImage from './EventImage';
import { Link } from 'react-router-dom';

const EventDetails = () => {
  const { id } = useParams();
  const [event, setEvent] = useState(null);
  const [newReview, setNewReview] = useState({ username: '', comment: '', rating: 5 });

  const dateOptions = {
    year: 'numeric',
    month: 'long',
    day: 'numeric',
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit',
    hour12: false, // 24 hodinový formát
    timeZoneName: 'short'
  };

  useEffect(() => {
    const fetchEventDetails = async () => {
      try {
        const response = await fetch(`http://localhost:5000/events/${id}`);
        const data = await response.json();
        setEvent(data);
      } catch (error) {
        console.error('Error fetching event details:', error);
      }
    };

    fetchEventDetails();
  }, [id]);

  const handleSubmitReview = async (e) => {
    e.preventDefault();
    try {
      const response = await fetch(`http://localhost:5000/events/${id}/reviews`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(newReview),
      });
      if (response.ok) {
        // Obnovení detailů akce aby se zobrazila nová recenze
        const refreshResponse = await fetch(`http://localhost:5000/events/${id}`);
        const refreshedData = await refreshResponse.json();
        setEvent(refreshedData);
        setNewReview({ username: '', comment: '', rating: 5 });
      }
    } catch (error) {
      console.error('Error submitting review:', error);
    }
  };

  if (!event) return <div className="container mx-auto px-4 py-8">Načítání...</div>;

  return (
    <div className="container mx-auto px-4 py-8">
      <div className="bg-white rounded-lg shadow-lg p-6 mb-8">
        <h2 className="text-2xl font-bold mb-4">{event.name}</h2>
        <div className="h-64 mb-4 rounded overflow-hidden">
          {event.image ? (
            <EventImage
              eventId={event.id}
              name={event.name}
              className="w-full h-full object-cover"
            />
          ) : (
            <div className="w-full h-full flex items-center justify-center bg-gray-200 text-gray-500">
              Obrázek není dostupný
            </div>
          )}
        </div>
        <p className="text-gray-600">Dostupné vstupenky: {event.available_tickets}</p>
        <p className="text-gray-600">
          Průměrné hodnocení: {event.average_rating ? event.average_rating.toFixed(1) : 'Zatím žádné hodnocení'}
        </p>
        {event.address ? (
          <p className="text-gray-600">Adresa: {event.address}</p>
        ) : (
          <p className="text-gray-600">Adresa akce neznámá</p>
        )}
        {event.date_of_event ? (
          <p className="text-gray-600">Datum konání: {event.date_of_event}</p>
        ) : (
          <p className="text-gray-600">Datum konání neznámo</p>
        )}
      </div>

      {/* Formulář pro přidání recenze */}
      <Link 
        to={`/event/${event.id}/addreview`}
        key={event.id} 
        className="text-white hover:text-indigo-200"
        >
        <button className="inline-flex justify-center rounded-md border border-transparent bg-indigo-600 py-2 px-4 text-sm font-medium text-white shadow-sm hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:ring-offset-2">
            Přidat recenzi
        </button>
      </Link>

      {/* Recenze */}
      <div className="bg-white rounded-lg shadow-lg p-6 mb-8">
        <h3 className="text-xl font-bold mb-4">Recenze ({event.reviews.length})</h3>
        <div className="mb-8">
          {event.reviews.map((review) => (
            <div key={review.id} className="border-b py-4 last:border-b-0">
              <div className="flex justify-between">
                <p className="font-medium">{review.username}</p>
                <div className="flex">
                  <p className="text-xs mr-3 text-center">{new Date(review.date_posted).toLocaleDateString("cs-CZ", dateOptions)}</p>
                  {[1, 2, 3, 4, 5].map((star) => (
                    <span key={star} className="text-yellow-400">
                      {star <= review.rating ? "★" : "☆"}
                    </span>
                  ))}
                </div>
              </div>
              <p className="text-gray-600 mt-2">{review.comment}</p>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
};

export default EventDetails;
