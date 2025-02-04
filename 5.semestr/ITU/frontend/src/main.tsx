/*
ITU Projekt
Webová aplikace - Akce pro lidi
Author: Marek Konečný (xkonec85)
        Kryštof Michálek (xmicha94)
Description: Hlavní soubor pro vstup do aplikace
*/
import { StrictMode } from 'react';
import { createRoot } from 'react-dom/client';
import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import { DndProvider } from 'react-dnd';
import { HTML5Backend } from 'react-dnd-html5-backend';
import 'react-calendar/dist/Calendar.css';
import './index.css';

import Header from "./components/Header";
import EventsPage from "./EventsPage";
import EventDetails from "./components/EventDetails";
import ProfilePage from "./ProfilePage";
import ProfileChangePage from "./ProfileChangePage";
import ProfileEditPage from "./ProfileEditPage";

import { UserProvider } from './UserContext';

createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <UserProvider> {/* Toto umožňuje získání informací o uživateli v jakékoliv části aplikace */}
      <DndProvider backend={HTML5Backend}>
        <Router>
          <div className="min-h-screen bg-gray-50">
            <Header /> {/* Hlavička je zde aby se zobrazila v jakékoliv části aplikace */}
            <Routes>
              <Route path="/" element={<EventsPage />} />
              <Route path="/event/:id" element={<EventDetails />} />
              <Route path="/profile" element={<ProfilePage />} />
              <Route path="/profilechange" element={<ProfileChangePage />} />
              <Route path="/profile/edit" element={<ProfileEditPage />} />
            </Routes>
          </div>
        </Router>
      </DndProvider>
    </UserProvider>
  </StrictMode>
);
