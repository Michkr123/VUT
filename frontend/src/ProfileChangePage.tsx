import React, { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';
import { useUser } from './UserContext';
import { Avatar, Skeleton, Snackbar, SnackbarCloseReason } from '@mui/material';
import { deepPurple } from '@mui/material/colors';

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

const ProfileChangePage = () => {
    const { login, setLogin } = useUser();

    const [isSnackbarOpen, setIsSnackbarOpen] = useState(false);

    const snackbarOpen = () => {
      setIsSnackbarOpen(true);
    }

    const snackbarClose = (event: React.SyntheticEvent | Event, reason?: SnackbarCloseReason) => {
      if (reason === 'clickaway') {
        return;
      }
      setIsSnackbarOpen(false);
    }

    const getSnackbarString = (user: string) => {
      if (user === 'user') return "Profil nastaven na uživatele!";
      if (user === 'worker') return "Profil nastaven na pracovníka!";
      if (user === 'admin') return "Profil nastaven na admina!";
    };

    const [userProfile, setUserProfile] = useState<Profile | null>(null);
    const [workerProfile, setWorkerProfile] = useState<Profile | null>(null);
    const [adminProfile, setAdminProfile] = useState<Profile | null>(null);

    useEffect(() => {
        const fetchProfiles = async () => {
          try {
            const response = await fetch(`http://localhost:5000/profiles/user`);
            const data = await response.json();
            setUserProfile(data);
          } catch (error) {
            console.error("Error fetching user profile:", error);
          }

          try {
            const response = await fetch(`http://localhost:5000/profiles/worker`);
            const data = await response.json();
            setWorkerProfile(data);
          } catch (error) {
            console.error("Error fetching worker profile:", error);
          }

          try {
            const response = await fetch(`http://localhost:5000/profiles/admin`);
            const data = await response.json();
            setAdminProfile(data);
          } catch (error) {
            console.error("Error fetching admin profile:", error);
          }
        };
    
        fetchProfiles();
      }, []);

      return (
        <div className="flex flex-col items-center bg-gray-200 min-h-screen p-32">
          <div className='grid grid-cols-3 grid-rows-1 gap-16'>

            {userProfile ?
              <div onClick={() => {setLogin("user"); snackbarOpen();}} className='flex flex-col items-center justify-center cursor-pointer active:opacity-60'>
                <Avatar sx={{ width: 200, height: 200, bgcolor: deepPurple[500]}} alt={userProfile?.nickname} src={userProfile?.image}>
                  {userProfile?.nickname}
                </Avatar>
                <p className='py-5'>Uživatel ({userProfile?.nickname})</p>
              </div>
              :
              <div className='flex flex-col items-center justify-center'>
                <Skeleton variant='circular' width={200} height={200} />
                <Skeleton className='py-5' variant='rounded' width={82} height={32} />
              </div>
            }

            {workerProfile ?
              <div onClick={() => {setLogin("worker"); snackbarOpen();}} className='flex flex-col items-center justify-center cursor-pointer active:opacity-60'>
                <Avatar sx={{ width: 200, height: 200, bgcolor: deepPurple[500]}} alt={workerProfile?.nickname} src={workerProfile?.image}>
                  {workerProfile?.nickname}
                </Avatar>
                <p className='py-5'>Pracovník ({workerProfile?.nickname})</p>
              </div>
              :
              <div className='flex flex-col items-center justify-center'>
                <Skeleton variant='circular' width={200} height={200} />
                <Skeleton className='py-5' variant='rounded' width={82} height={32} />
              </div>
            }

            {adminProfile ?
              <div onClick={() => {setLogin("admin"); snackbarOpen(); }} className='flex flex-col items-center justify-center cursor-pointer active:opacity-60'>
                <Avatar sx={{ width: 200, height: 200, bgcolor: deepPurple[500]}} alt={adminProfile?.nickname} src={adminProfile?.image}>
                  {adminProfile?.nickname}
                </Avatar> 
                <p className='py-5'>Administrátor ({adminProfile?.nickname})</p>
              </div>
              :
              <div className='flex flex-col items-center justify-center'>
                <Skeleton variant='circular' width={200} height={200} />
                <Skeleton className='py-5' variant='rounded' width={82} height={32} />
              </div>
            }

            <Snackbar
              open={isSnackbarOpen}
              anchorOrigin={{ vertical: "bottom", horizontal: "center" }}
              autoHideDuration={3000}
              onClose={snackbarClose}
              message={getSnackbarString(login)}
            />

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
