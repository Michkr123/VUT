import React, { useState, useEffect } from 'react';

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

interface ButtonProps {
  profile: Profile;
  eventId: number;
  onRegister: (login: string, eventId: number, role: 'visitor' | 'worker') => void;
  onUnregister: (login: string, eventId: number, role: 'visitor' | 'worker') => void;
}

const RegistrationButtons: React.FC<ButtonProps> = ({ profile, eventId, onRegister, onUnregister }) => {
  const [isVisitor, setIsVisitor] = useState(profile.visitorActions.includes(eventId));
  const [isWorker, setIsWorker] = useState(profile.workerActions.includes(eventId));

  useEffect(() => {
    setIsVisitor(profile.visitorActions.includes(eventId));
    setIsWorker(profile.workerActions.includes(eventId));
  }, [profile, eventId]);

  const handleVisitorClick = () => {
    if (isVisitor) {
      onUnregister(profile.login, eventId, 'visitor');
    } else {
      onRegister(profile.login, eventId, 'visitor');
    }
    setIsVisitor(!isVisitor);
  };

  const handleWorkerClick = () => {
    if (isWorker) {
      onUnregister(profile.login, eventId, 'worker');
    } else {
      onRegister(profile.login, eventId, 'worker');
    }
    setIsWorker(!isWorker);
  };

  return (
    <div className="flex justify-center my-4">
      <button
        className={`px-6 py-2 rounded-md mx-2 ${isVisitor ? 'bg-red-500 hover:bg-red-600' : 'bg-green-500 hover:bg-green-600'} text-white`}
        onClick={handleVisitorClick}
      >
        {isVisitor ? 'Odhlásit' : 'Přihlásit'}
      </button>
      <button
        className={`px-6 py-2 rounded-md mx-2 ${isWorker ? 'bg-red-500 hover:bg-red-600' : 'bg-green-500 hover:bg-green-600'} text-white`}
        onClick={handleWorkerClick}
      >
        {isWorker ? 'Odhlásit se jako dobrovolník' : 'Přihlásit se jako dobrovolník'}
      </button>
    </div>
  );
};

export default RegistrationButtons;
