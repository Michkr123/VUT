/*
ITU Projekt
Webová aplikace - Akce pro lidi
Author: Marek Konečný (xkonec85)
Description: Komponenta pro formulář pro úpravu akce
*/
import React, { useState, useEffect } from 'react';
import { Button, Dialog, DialogActions, DialogContent, DialogTitle, TextField, Select, MenuItem, InputLabel, FormControl } from '@mui/material';
import DraggableCategory from './DraggableCategory';
import CategoryDropZone from './CategoryDropZone';
import { EVENT_BUTTONS } from '../EventsPage';

// UpdateEventForm component
const UpdateEventForm = ({ open, onClose, eventId, onEventUpdated }) => {
  const [event, setEvent] = useState({ image: '', name: '', event_category: '', organizer: '', region: '', city: '', address: '', date_of_event: '', description: '' }); // State to store event data
  const [previewUrl, setPreviewUrl] = useState(null); // State to store image preview URL

  // useEffect hook to fetch the event data when the component mounts or eventId changes
  useEffect(() => {
    const fetchEvent = async () => {
      try {
        const response = await fetch(`http://localhost:5000/events/${eventId}`); // Fetch the event data from the server
        if (response.ok) {
          const data = await response.json(); // Parse the response as JSON
          setEvent(data); // Set the event data
          setPreviewUrl(data.image); // Set the preview URL
        }
      } catch (error) {
        console.error('Error fetching event:', error); // Log any errors
      }
    };

    if (eventId) {
      fetchEvent(); // Call the fetchEvent function if eventId is provided
    }
  }, [eventId]); // Dependency array to re-run the effect when eventId changes

  // Handle image change
  const handleImageChange = (e) => {
    const file = e.target.files[0];
    if (file) {
      const reader = new FileReader();
      reader.onloadend = () => {
        setEvent({ ...event, image: reader.result }); // Set the image data in event state
        setPreviewUrl(reader.result); // Set the preview URL
      };
      reader.readAsDataURL(file); // Read the file as a data URL
    }
  };

  // Handle category drop
  const handleDropCategory = (category) => {
    setEvent((prevEvent) => ({ ...prevEvent, event_category: category })); // Set the dropped category in event state
  };

  // Handle form submission (not shown in the provided code)
  const handleSubmitEvent = async (e) => {
    e.preventDefault();
    try {
      const response = await fetch(`http://localhost:5000/events/${eventId}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(event), // Send event data as JSON
      });
      if (response.ok) {
        onEventUpdated(); // Call onEventUpdated callback
        onClose(); // Close the form
      }
    } catch (error) {
      console.error('Error updating event:', error); // Log any errors
    }
  };

  return (
    <Dialog open={open} onClose={onClose} maxWidth="md" fullWidth>
      <DialogTitle>Upravit akci</DialogTitle>
      <DialogContent>
        <form onSubmit={handleSubmitEvent} className="space-y-4">
          {previewUrl && (
            <div>
              <img src={previewUrl} alt="Preview" style={{ height: '200px', width: 'auto' }} />
            </div>
          )}
          <div className="flex items-center space-x-2">
            <label htmlFor="image" className="text-gray-700">Obrázek:</label>
            <input type="file" accept="image/*" id="image" onChange={handleImageChange} />
          </div>
          <TextField label="Název akce" value={event.name} onChange={(e) => setEvent({ ...event, name: e.target.value })} fullWidth required />
          <TextField label="Organizátor" value={event.organizer} onChange={(e) => setEvent({ ...event, organizer: e.target.value })} fullWidth required />
          <FormControl fullWidth required>
            <InputLabel>Kraj</InputLabel>
            <Select value={event.region} onChange={(e) => setEvent({ ...event, region: e.target.value })}>
              <MenuItem value="praha">Praha</MenuItem>
              <MenuItem value="stredocesky">Středočeský</MenuItem>
              <MenuItem value="jihocesky">Jihočeský</MenuItem>
              <MenuItem value="plzensky">Plzeňský</MenuItem>
              <MenuItem value="karlovarsky">Karlovarský</MenuItem>
              <MenuItem value="ustecky">Ústecký</MenuItem>
              <MenuItem value="liberecky">Liberecký</MenuItem>
              <MenuItem value="kralovehradecky">Královéhradecký</MenuItem>
              <MenuItem value="pardubicky">Pardubický</MenuItem>
              <MenuItem value="vysocina">Vysočina</MenuItem>
              <MenuItem value="jihomoravsky">Jihomoravský</MenuItem>
              <MenuItem value="olomoucky">Olomoucký</MenuItem>
              <MenuItem value="moravskoslezsky">Moravskoslezský</MenuItem>
              <MenuItem value="zlinsky">Zlínský</MenuItem>
            </Select>
          </FormControl>
          <TextField label="Město" value={event.city} onChange={(e) => setEvent({ ...event, city: e.target.value })} fullWidth required />
          <TextField label="Adresa" value={event.address} onChange={(e) => setEvent({ ...event, address: e.target.value })} fullWidth required />
          <TextField label="Datum" type="date" value={event.date_of_event} onChange={(e) => setEvent({ ...event, date_of_event: e.target.value })} fullWidth required InputLabelProps={{ shrink: true }} />
          <TextField label="Popis" value={event.description} onChange={(e) => setEvent({ ...event, description: e.target.value })} fullWidth required />
          <CategoryDropZone onDrop={handleDropCategory} />
          <div className="flex flex-wrap gap-2">
            {EVENT_BUTTONS.map((category) => (
              <DraggableCategory key={category} category={`${category.emoji} ${category.label}`} />
            ))}
          </div>
        </form>
      </DialogContent>
      <DialogActions>
        <Button onClick={onClose} color="primary">Zpět</Button>
        <Button onClick={handleSubmitEvent} color="primary" variant="contained">Upravit</Button>
      </DialogActions>
    </Dialog>
  );
};

export default UpdateEventForm; // Export the component