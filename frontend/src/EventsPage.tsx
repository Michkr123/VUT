import React, { useState, useEffect } from "react";
import { Link } from "react-router-dom";

import EventImage from "./components/EventImage";

const EventsPage = () => {
  const [events, setEvents] = useState([]);
  const [address, setAddress] = useState("");

  useEffect(() => {
    const fetchEvents = async () => {
      try {
        const response = await fetch("http://localhost:5000/events");
        const data = await response.json();
        setEvents(data);
      } catch (error) {
        console.error("Error fetching events:", error);
      }
    };

    fetchEvents();
  }, []);

  return (
    <main className="container mx-auto px-4 py-8">

      {/* Filtry */}
      <div className="grid grid-cols-1 md:grid-cols-3 gap-4 mb-8">
        <select
          className="p-2 border rounded-md"
          value={address}
          onChange={(e) => setAddress(e.target.value)}
        >

          {/* Vyplnit možnosti zde podle stávajících akcí  */}

          <option value="">Vybrat lokaci</option>
          {/* <option value="praha">Praha</option>
          <option value="brno">Brno</option>
          <option value="ostrava">Ostrava</option> */}

        </select>

        <div className="bg-blue-100 p-4 rounded-lg">
          <p className="font-medium">Kalendář placeholder</p>
        </div>

      </div>

      {/* Kategorie podle kterých se bude filtrovat */}
      <div className="flex flex-wrap gap-2 mb-8">
        <button className="px-4 py-2 border rounded-md hover:bg-gray-100">
          🎵 Koncerty
        </button>
        <button className="px-4 py-2 border rounded-md hover:bg-gray-100">
          🍽️ Jídlo
        </button>
      </div>

      {/* Události */}
      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
        {events.map((event) => (
          <Link
            to={`/event/${event.id}`}
            key={event.id}
            className="border rounded-lg overflow-hidden bg-white shadow-sm hover:shadow-md transition-shadow"
          >
            <div className="relative h-48 bg-gray-200">

              {event.image ? (
                <EventImage
                  eventId={event.id}
                  name={event.name}
                  className="w-full h-full object-cover"
                />
              ) : (
                <div className="absolute inset-0 flex items-center justify-center text-gray-500 bg-gray-200">
                  Obrázek není dostupný
                </div>
              )}

              <div className="absolute top-4 left-4 bg-blue-600 text-white px-3 py-1 rounded">
                {event.name}
              </div>

            </div>
            
            {/* Pocet vstupenek, adresa akce, datum konání */}
            <div className="p-4">
              <div className="flex justify-between items-center">
                <p className="text-gray-600">
                  Dostupné vstupenky: {event.available_tickets}
                </p>
                <div className="flex">
                  {[1, 2, 3, 4, 5].map((star) => (
                    <span key={star} className="text-yellow-400">
                      {star <= (event.average_rating || 0) ? "★" : "☆"}
                    </span>
                  ))}
                </div>
              </div>
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
          </Link>
        ))}
      </div>
    </main>
  );
};

export default EventsPage;
