/*
ITU Projekt
Webová aplikace - Akce pro lidi
Author: Marek Konečný (xkonec85)
Description: Komponenta pro zobrazení obrázku akce
*/
import React, { useState, useEffect } from 'react';

// EventImage component
const EventImage = ({ eventId, name, className }) => {
  const [imageData, setImageData] = useState(null); // State to store the image data
  const [loading, setLoading] = useState(true); // State to track loading status
  const [error, setError] = useState(null); // State to track any errors

  // useEffect hook to fetch the image when the component mounts or eventId changes
  useEffect(() => {
    const fetchImage = async () => {
      try {
        setLoading(true); // Set loading to true before fetching
        const response = await fetch(`http://localhost:5000/events/${eventId}/image`); // Fetch the image from the server
        if (!response.ok) throw new Error('Failed to load image'); // Throw an error if the response is not ok
        const data = await response.json(); // Parse the response as JSON
        setImageData(data.image); // Set the image data
      } catch (err) {
        console.error('Error loading image:', err); // Log any errors
        setError(err); // Set the error state
      } finally {
        setLoading(false); // Set loading to false after fetching
      }
    };

    fetchImage(); // Call the fetchImage function
  }, [eventId]); // Dependency array to re-run the effect when eventId changes

  // Render a loading spinner if the image is still loading
  if (loading) {
    return (
      <div className={`flex items-center justify-center ${className} bg-gray-100`}>
        <div className="animate-spin rounded-full h-8 w-8 border-b-2 border-indigo-600"></div>
      </div>
    );
  }

  // Render an error message if there was an error or no image data
  if (error || !imageData) {
    return (
      <div className={`flex items-center justify-center ${className} bg-gray-200`}>
        <span className="text-gray-500">Obrázek není dostupný</span> {/* Display error message */}
      </div>
    );
  }

  // Render the image if it was successfully loaded
  return (
    <img src={imageData} alt={name} className={className} /> // Display the image
  );
};

export default EventImage; // Export the component
