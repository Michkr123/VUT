import { useState, useEffect } from "react";
import { Link } from "react-router-dom";
import { Rating } from "@mui/material";
import EventImage from "./components/EventImage";

interface Event {
  id: number;
  name: string;
  date_of_creation: string;
  available_tickets: number | null;
  date_of_event: string;
  event_category: EventCategory;
  address: string;
  image: string;
  organizer: string;
  region: Region;
  city: string;
  description: string;
  average_rating: number | null;
  review_count: number | null;
}

type EventCategory = 'entertainment' | 'education' | 'for_adults' | 'for_children' | 'workshop' | 'exposition';

type Region = 'praha' | 'stredocesky' | 'jihocesky' | 'plzensky' | 'karlovarsky' | 
              'ustecky' | 'liberecky' | 'kralovehradecky' | 'pardubicky' | 'vysocina' | 
              'jihomoravsky' | 'olomoucky' | 'moravskoslezsky' | 'zlinsky' | '';

interface EventButton {
  value: EventCategory;
  label: string;
  emoji: string;
}

const EVENT_BUTTONS: EventButton[] = [
  { value: 'entertainment', label: 'Zábava', emoji: '🎭' },
  { value: 'education', label: 'Vzdělání', emoji: '📚' },
  { value: 'for_adults', label: 'Pro dospělé', emoji: '🍷' },
  { value: 'for_children', label: 'Pro děti', emoji: '🧸' },
  { value: 'workshop', label: 'Workshop', emoji: '🛠️' },
  { value: 'exposition', label: 'Výstava', emoji: '🖼️' },
];

const REGIONS: { value: Region; label: string }[] = [
  { value: 'praha', label: 'Praha' },
  { value: 'stredocesky', label: 'Středočeský' },
  { value: 'jihocesky', label: 'Jihočeský' },
  { value: 'plzensky', label: 'Plzeňský' },
  { value: 'karlovarsky', label: 'Karlovarský' },
  { value: 'ustecky', label: 'Ústecký' },
  { value: 'liberecky', label: 'Liberecký' },
  { value: 'kralovehradecky', label: 'Královéhradecký' },
  { value: 'pardubicky', label: 'Pardubický' },
  { value: 'vysocina', label: 'Vysočina' },
  { value: 'jihomoravsky', label: 'Jihomoravský' },
  { value: 'olomoucky', label: 'Olomoucký' },
  { value: 'moravskoslezsky', label: 'Moravskoslezský' },
  { value: 'zlinsky', label: 'Zlínský' },
];

const EventsPage = () => {
  const [events, setEvents] = useState<Event[] | null>(null);
  const [filteredEvents, setFilteredEvents] = useState<Event[] | null>(null);
  const [region, setRegion] = useState<Region>("");
  const [category, setCategory] = useState<EventCategory | "">("");
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const fetchEvents = async () => {
      try {
        setIsLoading(true);
        setError(null);
        const response = await fetch("http://localhost:5000/events");
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const data = await response.json();
        setEvents(data);
        setFilteredEvents(data);
      } catch (error) {
        console.error("Error fetching events:", error);
        setError("Nepodařilo se načíst události. Zkuste to prosím později.");
      } finally {
        setIsLoading(false);
      }
    };

    fetchEvents();
  }, []);

  useEffect(() => {
    if (!events) return;

    const filtered = events.filter((event) => {
      const matchesRegion = !region || event.region === region;
      const matchesCategory = !category || event.event_category === category;
      return matchesRegion && matchesCategory;
    });

    setFilteredEvents(filtered);
  }, [events, region, category]);

  const capitalize = (str: string) => str.charAt(0).toUpperCase() + str.slice(1).toLowerCase();

  const handleFilterButtonClick = (value: EventCategory) => {
    setCategory(category === value ? "" : value);
  };

  const formatDate = (dateString: string) => {
    return new Date(dateString).toLocaleDateString('cs-CZ', {
      day: 'numeric',
      month: 'long',
      year: 'numeric',
    });
  };

  const getCategoryLabel = (category: EventCategory): string => {
    const button = EVENT_BUTTONS.find(btn => btn.value === category);
    return button ? `${button.emoji} ${button.label}` : category;
  };

  if (error) {
    return (
      <div className="container mx-auto px-4 py-8">
        <div className="bg-red-50 border border-red-200 text-red-700 px-4 py-3 rounded">
          {error}
        </div>
      </div>
    );
  }

  return (
    <main className="container mx-auto px-4 py-8">
      {/* Filtry */}
      <div className="grid grid-cols-1 md:grid-cols-3 gap-4 mb-8">
        <select
          className="p-2 border rounded-md"
          value={region}
          onChange={(e) => setRegion(e.target.value as Region)}
        >
          <option value="">Vybrat region</option>
          {REGIONS.map(({ value, label }) => (
            <option key={value} value={value}>{label}</option>
          ))}
        </select>
      </div>

      {/* Filtry kategorií */}
      <div className="flex flex-wrap gap-2 mb-8">
        {EVENT_BUTTONS.map((btn) => (
          <button
            key={btn.value}
            className={`px-4 py-2 border rounded-md transition-colors ${
              category === btn.value 
                ? 'bg-blue-600 text-white' 
                : 'hover:bg-gray-100 active:opacity-80'
            }`}
            onClick={() => handleFilterButtonClick(btn.value)}
          >
            {btn.emoji} {btn.label}
          </button>
        ))}
      </div>

      {/* Grid událostí */}
      {isLoading ? (
        <div className="flex justify-center items-center h-64">
          <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-blue-600"></div>
        </div>
      ) : filteredEvents?.length === 0 ? (
        <div className="flex justify-center items-center h-64">
          <p className="text-gray-500 text-xl">Žádné akce nevyhovují filtrům</p>
        </div>
      ) : (
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
          {filteredEvents?.map((event) => (
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

              <div className="p-5">
                <div className="flex flex-col gap-2">
                  <div className="flex justify-between items-start">
                    <div className="flex flex-col gap-1">
                      {/* <p className="text-gray-600">
                        {event.available_tickets 
                          ? `Dostupné vstupenky: ${event.available_tickets}`
                          : "Počet vstupenek neznámý"}
                      </p> */}
                      <p className="text-gray-600">
                        {event.region ? "Region: " + capitalize(event.region) : "Region akce neznámý"}
                      </p>
                      <p className="text-gray-600">
                        {event.address ? "Adresa: " + event.address : "Adresa akce neznámá"}
                      </p>
                      <p className="text-gray-600">
                        Datum konání: {event.date_of_event 
                          ? formatDate(event.date_of_event)
                          : "Datum konání neznámo"}
                      </p>
                      <p className="text-gray-600">
                        Kategorie: {getCategoryLabel(event.event_category)}
                      </p>
                    </div>
                    {event.average_rating !== null && (
                      <Rating 
                        name="read-only" 
                        value={event.average_rating} 
                        precision={0.5}
                        readOnly 
                        size="small"
                      />
                    )}
                  </div>
                </div>
              </div>

            </Link>
          ))}
        </div>
      )}
    </main>
  );
};

export default EventsPage;
