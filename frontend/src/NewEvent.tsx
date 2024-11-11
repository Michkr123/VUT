import React, { useState, useEffect } from 'react';
import { useParams } from 'react-router-dom';
import { Link } from 'react-router-dom';

const NewEvent = () =>{
  const [newEvent, setNewEvent] = useState({ image: '', name: '', event_category: '', organizer: '', region: '', city: '', address: '', date_of_event: '', description: '' });
  const [previewUrl, setPreviewUrl] = useState(null);

  const handleImageChange = (e) => {
    const file = e.target.files[0];
    if (file) {
      const reader = new FileReader();

      reader.onloadend = () =>{
        setNewEvent({ ...newEvent, image: reader.result });

        setPreviewUrl(reader.result);
      }
      
      reader.readAsDataURL(file);
    }
  };

  const handleSubmitEvent = async (e) => {
    e.preventDefault();
    try {
      const response = await fetch(`http://localhost:5000/events`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(newEvent),
      });
      if (response.ok) {
        window.location.replace(`/admin`);
      }
    } catch (error) {
      console.error('Error submitting review:', error);
    }
  };

  return(
    <div className="flex justify-center items-center h-screen">
      <form onSubmit={handleSubmitEvent} className="space-y-4">

        {previewUrl && (
          <div>
            <img src={previewUrl} alt="Preview" style={{height:"200px", width: "auto"}}/>
          </div>
        )}

        <div className="flex items-center space-x-2">
          <label htmlFor="image" className="text-gray-700">
            Obrázek: 
          </label>
          <input
            type="file"
            accept="image/*"
            id="image"
            onChange={handleImageChange}
            className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
            required
          />
        </div>

        <div className="flex items-center space-x-2">
          <label htmlFor="name" className="text-gray-700">
            Nazev akce: 
          </label>
          <input
            type="text"
            id="name"
            value={newEvent.name}
            onChange={(e) => setNewEvent({ ...newEvent, name: e.target.value })}
            className="pr-2 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
            required
          />
        </div>

        <div className="flex items-center space-x-2">
          <label htmlFor="event_category" className="text-gray-700">
            Typ akce: 
          </label>
          <select
            id="event_category"
            value={newEvent.event_category}
            onChange={(e) => setNewEvent({ ...newEvent, event_category: e.target.value})}
            className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
          >
            <option value="">Vyberte druh akce</option>
            <option value="entertainment">Zábava</option>
            <option value="education">Vzdělání</option>
            <option value="for_adults">Pro dospělé</option>
            <option value="for_children">Pro děti</option>
            <option value="workshop">Workshop</option>
            <option value="exposition">Výstava</option>
          </select>
        </div>

        <div className="flex items-center space-x-2">
          <label htmlFor="organizer" className="text-gray-700">
            Organizátor:
          </label>
          <input
            type="text"
            id="organizer"
            value={newEvent.organizer}
            onChange={(e) => setNewEvent({ ...newEvent, organizer: e.target.value })}
            className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
            required
          />
        </div>

        <div className="flex items-center space-x-2">
          <label htmlFor="eventType" className="text-gray-700">
            Kraj: 
          </label>
          <select
            id="eventType"
            value={newEvent.region}
            onChange={(e) => setNewEvent({ ...newEvent, region: e.target.value})}
            className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
          >
            <option value="">Vyberte kraj</option>
            <option value="praha">Praha</option>
            <option value="stredocesky">Středočeský</option>
            <option value="jihocesky">Jihočeský</option>
            <option value="plzensky">Plzeňský</option>
            <option value="karlovarsky">Karlovarský</option>
            <option value="ustecky">Ústecký</option>
            <option value="liberecky">Liberecký</option>
            <option value="kralovehradecky">Královéhradecký</option>
            <option value="pardubicky">Pardubický</option>
            <option value="vysocina">Vysočina</option>
            <option value="jihomoravsky">Jihomoravský</option>
            <option value="olomoucky">Olomoucký</option>
            <option value="moravskoslezsky">Moravskoslezský</option>
            <option value="zlinsky">Zlínský</option>
          </select>
        </div>

        <div className="flex items-center space-x-2">
          <label htmlFor="city" className="text-gray-700">
            Město:
          </label>
          <input
            type="text"
            id="city"
            value={newEvent.city}
            onChange={(e) => setNewEvent({ ...newEvent, city: e.target.value })}
            className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
            required
          />
        </div>

        <div className="flex items-center space-x-2">
          <label htmlFor="address" className="text-gray-700">
            Adresa:
          </label>
          <input
            type="text"
            id="address"
            value={newEvent.address}
            onChange={(e) => setNewEvent({ ...newEvent, address: e.target.value })}
            className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
            required
          />
        </div>

        <div className="flex items-center space-x-2">
          <label htmlFor="date_of_event" className="text-gray-700">
            Datum:
          </label>
          <input
            type="date"
            id="date_of_event"
            value={newEvent.date_of_event}
            onChange={(e) => setNewEvent({ ...newEvent, date_of_event: e.target.value })}
            className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
            required
          />
        </div>

        <div className="flex items-center space-x-2">
          <label htmlFor="description" className="text-gray-700">
            Popis:
          </label>
          <input
            type="text"
            id="description"
            value={newEvent.description}
            onChange={(e) => setNewEvent({ ...newEvent, description: e.target.value })}
            className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
            required
          />
        </div>

        <div className="flex justify-between mt-6 gap-10">
          <Link to="/admin" className="text-white hover:text-indigo-200">
            <button
              type="button"
              className="inline-flex justify-center rounded-md border border-transparent bg-gray-500 py-2 px-4 text-sm font-medium text-white shadow-sm hover:bg-gray-700 focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:ring-offset-2"
            >
              Zpět
            </button>
          </Link>

          <button
            type="submit"
            className="inline-flex justify-center rounded-md border border-transparent bg-indigo-600 py-2 px-4 text-sm font-medium text-white shadow-sm hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:ring-offset-2"
          >
            Vytvořit
          </button>
        </div>

      </form>
    </div>
  );
};

export default NewEvent;
