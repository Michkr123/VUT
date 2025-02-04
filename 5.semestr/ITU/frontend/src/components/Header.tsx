/*
ITU Projekt
Webová aplikace - Akce pro lidi
Author: Marek Konečný (xkonec85)
Description: Komponenta pro zobrazení hlavičky stránky
*/
import { Link } from 'react-router-dom';
import { useUser } from '../UserContext';

// Header component
const Header = () => {
  const { login } = useUser(); // Get the login function from the user context

  return (
    <header className="bg-indigo-600 p-6"> {/* Header container with background color and padding */}
      <div className="container mx-auto flex justify-between items-center"> {/* Flex container to align items */}
        <Link to="/" className="text-3xl font-bold text-white hover:text-indigo-200"> {/* Link to home page */}
          AKCE PRO LIDI {/* Site title */}
        </Link>
        <nav className="flex gap-4"> {/* Navigation container with gap between links */}
          <Link to="/" className="text-white hover:text-indigo-200"> {/* Link to events page */}
            Události
          </Link>
          <Link to="/profile" className="text-white hover:text-indigo-200"> {/* Link to profile page */}
            Profil
          </Link>
        </nav>
      </div>
    </header>
  );
};

export default Header; // Export the component
