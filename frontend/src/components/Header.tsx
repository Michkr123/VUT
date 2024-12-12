import { Link } from 'react-router-dom';
import { useUser } from '../UserContext';

const Header = () => {
  const { login } = useUser();

  return (
    <header className="bg-indigo-600 p-6">
      <div className="container mx-auto flex justify-between items-center">
        <Link to="/" className="text-3xl font-bold text-white hover:text-indigo-200">AKCE PRO LIDI</Link>
        <nav className="flex gap-4">
          <Link to="/" className="text-white hover:text-indigo-200">Události</Link>

          {/* Podle role uživatele zviditelňujeme/schováváme tlačitka */}
          {login === "worker" &&
            <Link to="/worker" className="text-white hover:text-indigo-200">Pracovník</Link>
          }

          <Link to="/profile" className="text-white hover:text-indigo-200">Profil</Link>
        </nav>
      </div>
    </header>
  );
};

export default Header;
