#ifndef CAMERA_
#define CAMERA_

#include "Frame.h"
#include <cmath>
#include <string>

struct CameraMatrices {
	glm::mat4 Proj;
	glm::mat4 View;
	glm::mat4 ViewProj;
	glm::mat4 ViewProjInv;
	glm::mat4 ViewProjNormal;
};

/**
 * @brief           Class to manage a camera in your virtual scene
 * @details         Verbose description of class details.
 */
class Camera {
	public:
		explicit Camera(std::string name= "");  // Creates a new Camera
		~Camera();  // Destroy a Camera

		/**
		 * @brief           Link the camera to the frame f (usually the root frame).
		 * @param f         The target frame
		 */
		void attachTo(Frame *f);

		//***********************       Camera position and orientation transformation       **********************//

		/**
		 * @brief           Define the position frame, using standard glut lookAt syntax.
		 * @details         Place the camera frame in the virtual scene : Point the Z-axis of camera frame toward point
		 *                  pointTo, using center as camera frame position and up as Y vector (pointed toward the top
		 *                  of the screen)
		 * @param pointTo   The 3D point faced by the camera
		 * @param center    The center of the camera in 3D space
		 * @param up        The up direction (the top of the screen) in 3D space.
		 */
		void lookAt(glm::vec3 pointTo, glm::vec3 center, glm::vec3 up);

		//***********************       Camera projection transformation       **********************//

		/**
		 * @brief           Set the projection matrix of camera
		 * @param m         The projection matrix
		 */
		void setProjectionMatrix(glm::mat4 & m);

		//***********************       Define a perspective projection       *********************//

		/**
		 * @brief               Set Perspective projection matrix using horizontal half field of view, pixel aspect ratio (width/height ratio of one pixel),  distance to near and far clipping planes.
		 * @param foV           To define half Field of View (in degrees) .
		 * @param aspectRatio   As aspect ratio.
		 * @param front         As distance of near clipping plane.
		 * @param back          As distance of back clipping plane.
		 */
		void setPerspectiveProjection(float foV, float aspectRatio, float front, float back );

		/**
		 * @brief           Set Perspective Frustum. <a href="http://en.wikipedia.org/wiki/Viewing_frustum.">Wiki link</a>
		 * @param left      As frustum left plane.
		 * @param right     As frustum right plane.
		 * @param bottom    As frustum bottom plane.
		 * @param top       As frustum top plane.
		 * @param near      As near clipping plane.
		 * @param far       As far clipping plane.
		 */
		void setFrustum(float left, float right, float bottom, float top, float near, float far);

		//***********************       Define orthographic projection       ********************//

		/**
		 * @brief           Set orthographic projection.
		 * @param left      As frustum left plane.
		 * @param right     As frustum right plane.
		 * @param bottom    As frustum bottom plane.
		 * @param top       As frustum top plane.
		 * @param near      As near clipping plane.
		 * @param far       As far clipping plane.
		 */
		void setOrthographicProjection(float left, float right, float bottom, float top, float near, float far);

		// ***********************       Members accessors       ***********************//

		/**
		 * @brief           Get distance of near clipping plane from camera frame center
		 * @return          Distance of near clipping plane
		 **/
		float getZnear() const;

		/**
		 * @brief           Get distance of far clipping plane from camera frame center
		 * @return          Distance of far clipping plane
		 **/
		float getZfar() const;

		/**
		 * @brief           Set distance of near clipping plane from camera frame center
		 * @param distance  Of near clipping plane
		 * @note            Recompute projection matrix
		 **/
		void setZnear(float n);

		/**
		 * @brief           Set distance of far clipping plane from camera frame center
		 * @param distance  Of far clipping plane
		 * @note            Recompute projection matrix
		 **/
		void setZfar(float f);

		/**
		 * @brief           Return field of view angle (in degrees) of camera.
		 * @return          Field of view angle
		 **/
		float getFoV() const;

		/**
		 * @brief           Set field of view angle (in degrees).
		 * @param half      Field of view angle
		 * @note            Recompute projection matrix
		 **/
		void setFoV(float v);

		/**
		 * @brief           Return pixel aspect ratio
		 * @return          Pixel aspect ratio
		 **/
		float getAspectRatio() const;

		/**
		 * @brief           Set pixel aspect ratio
		 * @param pixel     Aspect ratio
		 * @note            Recompute projection matrix
		 **/
		void setAspectRatio(float a);

		/**
		 * @brief           Return projection Matrix
		 * @return          Projection Matrix
		 **/
		glm::mat4 getProjectionMatrix();

		/**
		 * @brief           Return view matrix (transformation from root node to camera space)
		 * @return          View Matrix
		 **/
		glm::mat4 getViewMatrix();

		/**
		 * @brief           Set up camera position and orientation from matrix m
		 **/
		void setUpFromMatrix(glm::mat4 & m);

        /**
		 * @brief           Return true if camera moved since last update
		 * @return          True if camera moved since last update
		 **/
		bool updateNeeded();

		/**
		* @brief            Flag camera frame as having changed since last update
		* @param r          For flagging frame as needing update
		 **/
		virtual void setUpdate(bool r);

		/**
		* @brief            Return camera name
		 **/
		std::string getName();

		void updateBuffer();
		Frame* frame() { return this->m_Frame; };

	protected:
		Frame *m_Frame;		        // Camera position frame
		Frame *projection_frame;    // Camera projection frame

		glm::mat4 viewMatrix{};
		float aspectRatio;	        // Pixel aspect ratio (width/height ratio of one pixel)
		float foV;	                // Field of view of the camera
		float Znear;	            // Distance from camera frame center to near clipping plane
		float Zfar;		            // Distance from camera frame center to far clipping plane
        std::string m_Name;

    private:
        bool needUpdate;
        CameraMatrices matrices{};
};

#endif