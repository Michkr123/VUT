import React, { useState, useEffect } from 'react';
import { useNavigate  } from 'react-router-dom'
import { Link } from 'react-router-dom';

interface Profile {
    login: string;
    email: string;
    phone: string;
    nickname: string;
    image: string;
}

interface ProfileEditPageProps { login: string; }

const ProfileEditPage: React.FC<ProfileEditPageProps> = ({ login }) => {
    const [profile, setProfile] = useState<Profile | null>(null);
    const [email, setEmail] = useState('');
    const [phone, setPhone] = useState('');
    const [nickname, setNickname] = useState('');
    const [message, setMessage] = useState<string | null>(null);
    const navigate = useNavigate(); 

    useEffect(() => {
        const fetchProfile = async () => {
            try {
                const response = await fetch(`http://localhost:5000/profiles/${login}`);
                if (!response.ok) throw new Error("Failed to fetch profile.");
                const data = await response.json();
                setProfile(data);
                setEmail(data.email);
                setPhone(data.phone);
                setNickname(data.nickname);
            } catch (error) {
                console.error("Error fetching profile:", error);
                setMessage("Error loading profile. Please try again.");
            }
        };
    
        fetchProfile();
    }, [login]);

    const saveProfile = async () => {
        if (profile) {
            const updatedProfile = { ...profile, email, phone, nickname };
            try {
                const response = await fetch(`http://localhost:5000/profiles/${login}`, {
                    method: 'PUT',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(updatedProfile),
                });
                
                if (response.ok) {
                    setProfile(updatedProfile);  // Update profile state if successful
                    navigate(`/profile`);
                } else {
                    const errorData = await response.json();
                    setMessage(errorData.error || "Failed to update profile.");
                }
            } catch (error) {
                console.error("Error updating profile:", error);
                setMessage("Error updating profile. Please try again.");
            }
        }
    };

    return (
        <div>
          <div className="flex flex-col items-end bg-gray-200 p-8">
            <Link to="/profilechange" className="text-white hover:text-indigo-200">
              <button className="inline-flex justify-center rounded-md border border-transparent bg-indigo-600 py-2 px-4 text-sm font-medium text-white shadow-sm hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:ring-offset-2">
                Změnit profil 
              </button>
            </Link>
          </div>

          <div className="flex flex-col items-center bg-gray-200 min-h-screen p-8">
              <img
                  src={profile?.image}
                  alt='image'
              />

              <div className="grid grid-cols-2 grid-rows-5 gap-4 p-4">
                  <p className="text-right pr-2">Login: </p>
                  <p className="pl-2"> {profile?.login} </p>
                  
                  <p className="text-right pr-2">E-mail: </p> 
                  <input
                      type="email"
                      className="pl-2 border border-gray-300 rounded"
                      value={email}
                      onChange={(e) => setEmail(e.target.value)}
                  />

                  <p className="text-right pr-2">Tel: </p>
                  <input
                      type="tel"
                      className="pl-2 border border-gray-300 rounded"
                      value={phone}
                      onChange={(e) => setPhone(e.target.value)}
                  />

                  <p className="text-right pr-2">Jméno: </p>
                  <input
                      type="text"
                      className="pl-2 border border-gray-300 rounded"
                      value={nickname}
                      onChange={(e) => setNickname(e.target.value)}
                  />
                  
                  <p className="text-center col-span-2">Kalendář mých akcí: </p>
              </div>

              {message && <p className="text-center text-red-500">{message}</p>}

              <div className="flex justify-between mt-6 gap-10">
                  <Link to="/profile" className="text-white hover:text-indigo-200">
                      <button className="bg-gray-300 text-black py-2 px-4 rounded-lg hover:bg-gray-400">
                          Zpět
                      </button>
                  </Link>
                  <button
                      onClick={saveProfile}  // Call saveProfile when clicked
                      className="inline-flex justify-center rounded-md border border-transparent bg-indigo-600 py-2 px-4 text-sm font-medium text-white shadow-sm hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:ring-offset-2"
                  >
                      Upravit 
                  </button>
              </div>
          </div>
        </div>
    );
};

export default ProfileEditPage;
