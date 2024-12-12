import React, { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';
import { useUser } from './UserContext';
import { Avatar } from '@mui/material';
import { deepPurple } from '@mui/material/colors';
import EventCalendar from './components/EventCalendar';  // Import your EventCalendar component

interface Profile {
  login: string;
  email: string;
  phone: string;
  nickname: string;
  image: string;
}

const ProfilePage: React.FC = () => {
  const [profile, setProfile] = useState<Profile | null>(null);
  const { login } = useUser();

  const [events, setEvents] = useState([
      {
          date: new Date(),
          title: 'First Event',
          id: '1',
          popupContent: (
              <>
                  <p>Details about the first event.</p>
              </>
          ),
      },
      {
          date: new Date(),
          title: 'Second Event',
          id: '2',
          popupContent: (
              <>
                  <p>Details about the second event.</p>
              </>
          ),
      },
      // Add more events as needed
  ]);

  useEffect(() => {
      const fetchProfile = async () => {
        try {
          const response = await fetch(`http://localhost:5000/profiles/${login}`);
          const data = await response.json();
          setProfile(data);
        } catch (error) {
          console.error("Error fetching profiles:", error);
        }
      };
  
      fetchProfile();
    }, [login]);  // Add login as a dependency for useEffect

    return (
      <div>
        <div className='flex flex-col items-end bg-gray-200 p-8'>
          <Link to="/profilechange" className="text-white hover:text-indigo-200">
            <button className="inline-flex justify-center rounded-md border border-transparent bg-indigo-600 py-2 px-4 text-sm font-medium text-white shadow-sm hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:ring-offset-2">
              Změnit profil 
            </button>
          </Link>
        </div>
        <div className="flex flex-col items-center bg-gray-200 min-h-screen p-8">
            <Avatar sx={{ width: 200, height: 200, bgcolor: deepPurple[500]}} alt={profile?.nickname} src={profile?.image}>
              {profile?.nickname}
            </Avatar>

            <div className="grid grid-cols-2 grid-rows-5 gap-4 p-4">
                <p className="text-right pr-2">Login: </p>
                <p className="pl-2"> {profile?.login} </p>
                <p className="text-right pr-2">E-mail: </p> 
                <p className="pl-2"> {profile?.email} </p>
                <p className="text-right pr-2">Tel: </p> 
                <p className="pl-2"> {profile?.phone} </p>
                <p className="text-right pr-2" >Jméno: </p> 
                <p className="pl-2"> {profile?.nickname} </p>

                <p className="text-center col-span-2">Kalendář mých akcí: </p>
            </div>
            <div className="flex justify-between mt-6 gap-10">
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
            {/* Add the EventCalendar component here */}
            <div style={{ width: '100%', marginTop: '20px' }}>
                <EventCalendar
                    dataSource={events}
                    pallet={{ primary: '#32d3a2', secondary: '#2343d3' }}
                    onDataChange={(newEvents) => setEvents(newEvents)}
                />
            </div>
        </div>
      </div>
    );
};

export default ProfilePage;
