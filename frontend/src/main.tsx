import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import './index.css'

import Header from "./components/Header";
import EventsPage from "./EventsPage";
import EventDetails from "./components/EventDetails";
import WorkerPage from "./WorkerPage";
import AdminPage from "./AdminPage";
import NewEvent from './NewEvent';
import ProfilePage from "./ProfilePage";
import ProfileChangePage from "./ProfileChangePage";
import AddReviewPage from "./AddReviewPage";
import ProfileEditPage from "./ProfileEditPage";

import { UserProvider } from './UserContext';

createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <UserProvider> {/* Toto umožňuje získání informací o uživateli v jakékoliv části aplikace */}
      <Router>
        <div className="min-h-screen bg-gray-50">
          <Header /> {/* Hlavička je zde aby se zobrazila v jakékoliv části aplikace */}
          <Routes>
            <Route path="/" element={<EventsPage />} />
            <Route path="/event/:id" element={<EventDetails />} />
            <Route path="/worker" element={<WorkerPage />} />
            <Route path="/admin" element={<AdminPage />} />
            <Route path="/admin/new_event" element={<NewEvent />} />
            <Route path="/profile" element={<ProfilePage />} />
            <Route path="/profilechange" element={<ProfileChangePage />} />
            <Route path="/event/:id/addreview" element={<AddReviewPage />} />
            <Route path="/profile/edit" element={<ProfileEditPage />} />
          </Routes>
        </div>
      </Router>
    </UserProvider>
  </StrictMode>,
)
