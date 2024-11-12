import React, { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';

interface Profile {
    login: string;
    email: string;
    phone: string;
    nickname: string;
    image: string;
}

interface ProfileChangePageProps { login: string; }

const ProfileChangePage: React.FC<ProfileChangePageProps>= ({ login }) => {
    const [profile, setProfile] = useState<Profile | null>(null);

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
      }, []);

      return (
        
        <div className="flex flex-col items-center bg-gray-200 min-h-screen p-32">
          <div className='grid grid-cols-3 grid-rows-2'>
            <Link to="/" className="text-white hover:text-indigo-200">
                <div className='p-20 m-10 bg-black rounded-full w-8 h-8'>
                <img className=''
                    src={profile?.image}
                />
                </div>
            </Link>

            <Link to="/worker" className="text-white hover:text-indigo-200">
                <div className='p-20 m-10 bg-black rounded-full'>
                <img
                    src={profile?.image}
                />
                </div>
            </Link>

            <Link to="/admin" className="text-white hover:text-indigo-200">
                <div className='p-20 m-10 bg-black rounded-full'>
                <img
                    src={profile?.image}
                />
                </div>
            </Link>

            <div className='text-center h-8'>
              User
            </div>
            <div className='text-center h-8'>
              Worker
            </div>
            <div className='text-center h-8'>
              Admin
            </div>
          </div>
          <div className='p-10'>
                  <Link to="/profile" className="text-white hover:text-indigo-200">
                      <button className="bg-gray-300 text-black py-2 px-4 rounded-lg hover:bg-gray-400">
                          Zpět
                      </button>
                  </Link>
              </div>
        </div>
      );
};

export default ProfileChangePage;