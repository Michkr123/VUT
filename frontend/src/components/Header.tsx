import React from 'react';
import { Link } from 'react-router-dom';

const Header = () => {
  return (
    <header className="bg-indigo-600 p-6">
      <div className="container mx-auto flex justify-between items-center">
        <Link to="/" className="text-3xl font-bold text-white hover:text-indigo-200">AKCE PRO LIDI</Link>
        <nav className="flex gap-4">
          <Link to="/" className="text-white hover:text-indigo-200">Události</Link>
          <Link to="/admin/new_event" className="text-white hover:text-indigo-200">Nová akce</Link>
          <Link to="/worker" className="text-white hover:text-indigo-200">Pracovník</Link>
          <Link to="/admin" className="text-white hover:text-indigo-200">Admin</Link>
        </nav>
      </div>
    </header>
  );
};

export default Header;
