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
import AddReviewPage from "./AddReviewPage";

const login = "xmicha94";

createRoot(document.getElementById('root')!).render(
  <StrictMode>
    {/* Routování mezi jednotlivými časti aplikace */}
    <Router>
      <div className="min-h-screen bg-gray-50">
        <Header /> {/* Hlavička je zde aby se zobrazila v jakékoliv části aplikace */}
        <Routes>
          <Route path="/" element={<EventsPage />} />
          <Route path="/event/:id" element={<EventDetails />} />
          <Route path="/worker" element={<WorkerPage />} />
          <Route path="/admin" element={<AdminPage />} />
          <Route path="/admin/new_event" element={<NewEvent />} />
          <Route path="/profile" element={<ProfilePage login={login} />} />
          <Route path="/event/:id/addreview" element={<AddReviewPage login={login} />} />
        </Routes>
      </div>
    </Router>
  </StrictMode>,
)
