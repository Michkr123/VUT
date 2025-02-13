/*
ITU Projekt
Webová aplikace - Akce pro lidi
Author: Michálek Kryštof (xmicha94)
Description: Profilová stránka uživatele s možností upravení údajů.
*/


import { useState, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import { Link } from 'react-router-dom';
import { Avatar } from '@mui/material';
import { deepPurple } from '@mui/material/colors';
import { useUser } from './UserContext';

interface Profile {
    login: string;
    email: string;
    phone: string;
    nickname: string;
    image: string;
    worker: boolean;
    admin: boolean;
    visitorActions: number[];
    workerActions: number[];
}

const ProfileEditPage = () => {
    const { login } = useUser();
    const [profile, setProfile] = useState<Profile | null>(null);
    const [email, setEmail] = useState('');
    const [phone, setPhone] = useState('');
    const [nickname, setNickname] = useState('');
    const [message, setMessage] = useState<string | null>(null);
    const navigate = useNavigate();

    // Načtení profilových dat při načtení komponenty
    useEffect(() => {
        const fetchProfile = async () => {
            try {
                const response = await fetch(`http://localhost:5000/profiles/${login}`);
                if (!response.ok) throw new Error("Nepodařilo se načíst profil.");
                const data = await response.json();
                setProfile(data);
                setEmail(data.email);
                setPhone(data.phone);
                setNickname(data.nickname);
            } catch (error) {
                console.error("Chyba při načítání profilu:", error);
                setMessage("Chyba při načítání profilu. Zkuste to prosím znovu.");
            }
        };

        fetchProfile();
    }, [login]);

    // Uložení aktualizovaných údajů do profilu
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
                    setProfile(updatedProfile);  
                    navigate(`/profile`);
                } else {
                    const errorData = await response.json();
                    setMessage(errorData.error || "Nepodařilo se aktualizovat profil.");
                }
            } catch (error) {
                console.error("Chyba při aktualizaci profilu:", error);
                setMessage("Chyba při aktualizaci profilu. Zkuste to prosím znovu.");
            }
        }
    };

    return (
        <div>
            {/* Hlavička pro úpravu profilu */}
            <div className="flex flex-col items-end bg-gray-200 p-8">
                <Link to="/profilechange" className="text-white hover:text-indigo-200">
                    <button className="inline-flex justify-center rounded-md border border-transparent bg-indigo-600 py-2 px-4 text-sm font-medium text-white shadow-sm hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:ring-offset-2">
                        Změnit profil 
                    </button>
                </Link>
            </div>

            {/* Zobrazení profilu a formulář pro úpravy */}
            <div className="flex flex-col items-center bg-gray-200 min-h-screen p-8">
                <Avatar sx={{ width: 200, height: 200, bgcolor: deepPurple[500] }} alt={profile?.nickname} src={profile?.image}>
                    {profile?.nickname}
                </Avatar>

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

                {/* Zobrazení chybové zprávy, pokud existuje */}
                {message && <p className="text-center text-red-500">{message}</p>}

                {/* Akční tlačítka */}
                <div className="flex justify-between mt-6 gap-10">
                    <Link to="/profile" className="text-white hover:text-indigo-200">
                        <button className="bg-gray-300 text-black py-2 px-4 rounded-lg hover:bg-gray-400">
                            Zpět
                        </button>
                    </Link>
                    <button
                        onClick={saveProfile}  // Zavolání saveProfile při kliknutí
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
