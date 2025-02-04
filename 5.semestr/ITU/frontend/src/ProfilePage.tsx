/*
ITU Projekt
Webová aplikace - Akce pro lidi
Author: Michálek Kryštof (xmicha94)
Description: Profilová stránka uživatele s kalendářem událostí.
*/

import React, { useState, useEffect } from 'react';
import { Link } from 'react-router-dom'; // Pro navigaci mezi stránkami
import { useUser } from './UserContext'; // Kontext pro získání informací o přihlášeném uživateli
import { Avatar } from '@mui/material';
import { deepPurple } from '@mui/material/colors';
import { Calendar, momentLocalizer } from 'react-big-calendar';
import moment from 'moment';
import 'react-big-calendar/lib/css/react-big-calendar.css'; // Styl pro kalendář

interface Profile {
  login: string;
  email: string;
  phone: string;
  nickname: string;
  image: string;
  visitorActions: number[]; // Seznam akcí, kde uživatel je návštěvník
  workerActions: number[]; // Seznam akcí, kde uživatel je pracovník
}

interface Event {
  id: number;
  name: string;
  date_of_event: string; // Datum akce ve formátu 'YYYY-MM-DD'
  description: string;
}

const ProfilePage: React.FC = () => {
  const [profile, setProfile] = useState<Profile | null>(null); // Stav pro profil uživatele
  const [events, setEvents] = useState<any[]>([]); // Seznam událostí pro kalendář
  const { login } = useUser(); // Získání loginu přihlášeného uživatele z kontextu

  const localizer = momentLocalizer(moment); // Inicializace lokalizátoru pro kalendář

  useEffect(() => {
    const fetchProfileAndEvents = async () => {
      try {
        // Načtení profilu uživatele
        const profileResponse = await fetch(`http://localhost:5000/profiles/${login}`);
        const profileData: Profile = await profileResponse.json();
        setProfile(profileData);

        // Načtení všech událostí
        const eventsResponse = await fetch(`http://localhost:5000/events`);
        const allEvents: Event[] = await eventsResponse.json();

        // Filtrace událostí, které uživatel navštívil nebo na kterých pracoval
        const attendedEvents = allEvents.filter((event) => 
          profileData.workerActions.includes(event.id) || profileData.visitorActions.includes(event.id)
        );

        // Mapování událostí pro kalendář
        const eventData = attendedEvents.map((event) => {
          const isWorkerEvent = profileData.workerActions.includes(event.id);
          const isVisitorEvent = profileData.visitorActions.includes(event.id);

          return {
            eventID: event.id,
            eventName: event.name,
            role: isWorkerEvent ? 'worker' : isVisitorEvent ? 'visitor' : 'none', // Určení role na základě akcí
            start: new Date(event.date_of_event), // Převod data události na objekt Date
            end: new Date(event.date_of_event), // Předpoklad, že akce končí ve stejný den
            title: event.name,
          };
        });

        setEvents(eventData);
      } catch (error) {
        console.error('Error fetching profile or events:', error); // Chyba při načítání dat
      }
    };

    fetchProfileAndEvents(); // Zavolání funkce pro načtení dat
  }, [login]); // Spuštění efektu při změně loginu

  if (!profile) {
    return <div>Loading...</div>; // Zobrazení "Loading..." dokud se profil načítá
  }

  return (
    <div>
      <div className="flex flex-col items-end bg-gray-200 p-8">
        {/* Tlačítko pro změnu profilu */}
        <Link to="/profilechange" className="text-white hover:text-indigo-200">
          <button className="inline-flex justify-center rounded-md border border-transparent bg-indigo-600 py-2 px-4 text-sm font-medium text-white shadow-sm hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:ring-offset-2">
            Změnit profil
          </button>
        </Link>
      </div>
      <div className="flex flex-col items-center bg-gray-200 min-h-screen p-8">
        {/* Zobrazení avatara uživatele */}
        <Avatar
          sx={{ width: 200, height: 200, bgcolor: deepPurple[500] }}
          alt={profile.nickname}
          src={profile.image}
        >
          {profile.nickname}
        </Avatar>

        {/* Zobrazení informací o uživatelském profilu */}
        <div className="grid grid-cols-2 grid-rows-5 gap-4 p-4">
          <p className="text-right pr-2">Login: </p>
          <p className="pl-2">{profile.login}</p>
          <p className="text-right pr-2">E-mail: </p>
          <p className="pl-2">{profile.email}</p>
          <p className="text-right pr-2">Tel: </p>
          <p className="pl-2">{profile.phone}</p>
          <p className="text-right pr-2">Jméno: </p>
          <p className="pl-2">{profile.nickname}</p>
        </div>

        {/* Tlačítka pro návrat a editaci profilu */}
        <div className="flex justify-between gap-10">
          <Link to="/" className="text-white hover:text-indigo-200">
            <button className="bg-gray-300 text-black py-2 px-4 rounded-lg hover:bg-gray-400">
              Zpět
            </button>
          </Link>
          <Link to="/profile/edit" className="text-white hover:text-indigo-200">
            <button className="inline-flex justify-center rounded-md border border-transparent bg-indigo-600 py-2 px-4 text-sm font-medium text-white shadow-sm hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:ring-offset-2">
              Upravit
            </button>
          </Link>
        </div>

        {/* Zobrazení kalendáře s událostmi */}
        <p className="text-center col-span-2 pt-10">Kalendář mých akcí: </p>
        <div style={{ width: '100%', marginTop: '20px' }}>
          <Calendar
            localizer={localizer}
            events={events}
            startAccessor="start"
            endAccessor="end"
            titleAccessor="title"
            style={{
              height: 500,
              backgroundColor: 'white',
            }}
            eventPropGetter={(event) => ({
              style: {
                backgroundColor: event.role === 'worker' ? 'green' : event.role === 'visitor' ? 'blue' : 'gray',
                color: 'white',
              },
            })}
            onSelectEvent={(event) => {
              window.location.href = `/event/${event.eventID}`; // Při kliknutí na událost přejít na detail
            }}
          />
        </div>
      </div>
    </div>
  );
};

export default ProfilePage;
